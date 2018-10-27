// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UWorldGenerator.h"
#include "UComputerTypeAsset.h"
#include "AssetRegistryModule.h"
#include "USystemContext.h"
#include "UPeacegateFileSystem.h"

FString UWorldGenerator::GenerateRandomName(const FRandomStream& InGenerator, const TArray<FString> InFirstNames, TArray<FString> InLastNames)
{
	FString first = InFirstNames[InGenerator.RandRange(0, InFirstNames.Num() - 1)];
	FString last = InLastNames[InGenerator.RandRange(0, InLastNames.Num() - 1)];

	return first + TEXT(" ") + last;
}

int32 UWorldGenerator::GetSeedFromString(const FString& InSeedString)
{
	if (InSeedString.IsNumeric())
	{
		return FCString::Atoi(*InSeedString);
	}

	if (InSeedString.IsEmpty())
		return 8675309;

	TArray<TCHAR> Chars = InSeedString.GetCharArray();

	// Below algorithm is written by Declan Hoare (NetwideRogue).

	int result = 0;
	int charCount = Chars.Num();

	for (int i = 0; i < charCount; i++)
	{
		uint8 c = (uint8)Chars[i];
		result ^= c << (((i + 1) & 3) << 3);
	}

	return result;
}

FString UWorldGenerator::GenerateRandomPassword(const FRandomStream& InGenerator, int InLength)
{
	FString NewPassword;

	const FString Chars = TEXT("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890!@#$%^&*()_+-=`~]}[{'\"\\|/?.>,<");

	TArray<TCHAR> CharArr = Chars.GetCharArray();

	for (int i = 0; i < InLength; i++)
	{
		TCHAR Char = CharArr[InGenerator.RandRange(0, CharArr.Num() - 1)];

		NewPassword = NewPassword.AppendChar(Char);
	}

	return NewPassword;
}

FRandomStream UWorldGenerator::GetRandomNumberGenerator(int32 InSeed)
{
	return FRandomStream(InSeed);
}

void UWorldGenerator::GenerateCharacters(UPARAM(Ref) TArray<FPeacenetIdentity>& CharacterArray, UPARAM(Ref) TArray<FComputer>& ComputerArray, const FRandomStream& InGenerator, const TArray<FString> InFirstNames, TArray<FString> InLastNames)
{
	// Get the Asset Registry
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

	// A place to store computer type asset data
	TArray<FAssetData> ComputerTypeAssets;

	// List of markov training data assets
	TArray<FAssetData> MarkovAssets;

	AssetRegistryModule.Get().GetAssetsByClass(TEXT("ComputerTypeAsset"), ComputerTypeAssets, true);
	AssetRegistryModule.Get().GetAssetsByClass(TEXT("MarkovTrainingDataAsset"), MarkovAssets, true);

	TArray<UComputerTypeAsset*> ComputerTypes;
	TArray<UMarkovTrainingDataAsset*> MarkovTrainingData;

	// Load all the computer type assets, we'll need them for later.
	for (auto& Asset : ComputerTypeAssets)
	{
		ComputerTypes.Add((UComputerTypeAsset*)Asset.GetAsset());
	}

	// Load all the computer type assets, we'll need them for later.
	for (auto& Asset : MarkovAssets)
	{
		MarkovTrainingData.Add((UMarkovTrainingDataAsset*)Asset.GetAsset());
	}

	UMarkovChain* HostnameGenerator = CreateMarkovChain(FilterTrainingData(MarkovTrainingData, EMarkovTrainingDataUsage::Hostnames), InGenerator);

	// Retrieve the number of countries in the game.
	int CountryCount = (int)ECountry::Num_Countries;

	// The MINIMUM amount of computers that will generate in the world.
	const int MIN_WORLD_SIZE = 200;

	for (UComputerTypeAsset* ComputerType : ComputerTypes)
	{
		// TODO: ComputerType->Rarity should be ComputerType->Chance.
		float Rarity = ComputerType->Rarity;

		// The amount of computers to generate of this type.
		int ComputersToGenerate = (int)(MIN_WORLD_SIZE * Rarity);

		// Let's generate them.
		for (int i = 0; i < ComputersToGenerate; i++)
		{
			FComputer NewComputer;

			// ID becomes current world computer count.
			NewComputer.ID = ComputerArray.Num();

			// Computer type matches the ID in the asset.
			NewComputer.ComputerType = ComputerType->InternalID;

			// TODO: proper hostname generator
			NewComputer.Hostname = FText::FromString(GenerateWordString(HostnameGenerator, 1));

			// Now, we format the filesystem.
			TArray<FFolder> NewFS;
			UFileUtilities::FormatFilesystem(NewFS);

			NewComputer.Filesystem = NewFS;

			// Now, we create a root user account.
			FUser RootUser;
			RootUser.Username = FText::FromString(TEXT("root"));

			// TODO: skill-based password generation.
			RootUser.Password = FText::FromString(UWorldGenerator::GenerateRandomPassword(InGenerator, 64));

			// Root user needs to be admin
			RootUser.Domain = EUserDomain::Administrator;

			// and ID 0
			RootUser.ID = 0;

			// Add user account
			NewComputer.Users.Add(RootUser);

			// BEGIN TODO: Non-root user generation
			// END TODO


			// Create a system context.
			USystemContext* SysCtx = NewObject<USystemContext>();
			SysCtx->Computer = NewComputer;

			UWorldGenerator::GenerateSystemDirectories(SysCtx);


			// As the system context works its magic, it's been updaying its copy of our new computer.
			// Emphasis on copy. We're going to copy its updated computer back into our scope so we can
			// spawn it into the world.

			// Do NOT perform any actions using the above system context after this point. They won't be saved.
			NewComputer = SysCtx->Computer;

			// Add the computer to the world.
			ComputerArray.Add(NewComputer);

		}
	}
}

void UWorldGenerator::GenerateSystemDirectories(USystemContext * InSystemContext)
{
	// This lets us get a filesystem context as if we were logged into Peacegate OS as the root user we created above.
	// We're still generating the computer - there's no Peacegate yet - we're about to "install" it.
	// However, since we're in the C++ land, we can create a Peacegate context before Peacegate exists so we can generate the system properly.

	UPeacegateFileSystem* Filesystem = InSystemContext->GetFilesystem(0);

	EFilesystemStatusCode InstallStatusCode = EFilesystemStatusCode::OK;

	// CODE-DUPLICATION, TODO: create these dynamically when generating lootable files
	Filesystem->CreateDirectory(TEXT("/bin"), InstallStatusCode);
	Filesystem->CreateDirectory(TEXT("/etc"), InstallStatusCode);
	Filesystem->CreateDirectory(TEXT("/usr"), InstallStatusCode);
	Filesystem->CreateDirectory(TEXT("/var"), InstallStatusCode);
	Filesystem->CreateDirectory(TEXT("/run"), InstallStatusCode);
	Filesystem->CreateDirectory(TEXT("/tmp"), InstallStatusCode);
	Filesystem->CreateDirectory(TEXT("/etc/peacegate"), InstallStatusCode);

	// Now we can look through all the users and create their home directories.
	for (auto User : InSystemContext->Computer.Users)
	{
		FString Home = InSystemContext->GetUserHomeDirectory(User.ID);

		Filesystem->CreateDirectory(Home, InstallStatusCode);

		// TODO, CODE-DUPLICATION, REPETITIVE STRAIN INJURY WARNING: We should create these dynamically based on file assets that are randomly picked to place in the filesystem.
		Filesystem->CreateDirectory(Home + TEXT("/Desktop"), InstallStatusCode);
		Filesystem->CreateDirectory(Home + TEXT("/Documents"), InstallStatusCode);
		Filesystem->CreateDirectory(Home + TEXT("/Downloads"), InstallStatusCode);
		Filesystem->CreateDirectory(Home + TEXT("/Pictures"), InstallStatusCode);
		Filesystem->CreateDirectory(Home + TEXT("/Videos"), InstallStatusCode);
		Filesystem->CreateDirectory(Home + TEXT("/Music"), InstallStatusCode);
		Filesystem->CreateDirectory(Home + TEXT("/.peacegate"), InstallStatusCode);

	}

}

UMarkovChain * UWorldGenerator::CreateMarkovChain(const TArray<FString> InTrainingData, FRandomStream InRandomStream)
{
	UMarkovChain* NewChain = NewObject<UMarkovChain>();
	NewChain->Init(InTrainingData, 3, InRandomStream);

	return NewChain;
}

FString UWorldGenerator::GenerateWordString(UMarkovChain * InMarkovChain, int Length)
{
	FString Out;
	for (int i = 0; i < Length; i++)
		Out.Append(InMarkovChain->GetMarkovString(0));
	return Out;
}

void FMarkovSource::SetCount(int InCount)
{
	this->Chars = TArray<TCHAR>();

	for (int i = 0; i < InCount; i++)
	{
		Chars.Add(TEXT('\0'));
	}
}

void FMarkovSource::Rotate(TCHAR NextChar)
{
	check(Chars.Num() > 0);

	for (int i = 0; i < Chars.Num() - 1; i++)
	{
		Chars[i] = Chars[i + 1];
	}
	Chars[Chars.Num() - 1] = NextChar;
	Data = ToString();
}

bool FMarkovSource::IsLessThan(const FMarkovSource& OtherSource)
{
	int i = 0;
	for (int i = 0; i < Chars.Num() - 1; i++)
	{
		if (Chars[i] != OtherSource.Chars[i]) break;
	}
	return Chars[i] < OtherSource.Chars[i];
}

bool FMarkovSource::IsStartSource()
{
	for (auto Char : Chars)
	{
		if (Char != TEXT('\0')) return false;
	}
	return true;
}

bool FMarkovSource::operator==(const FMarkovSource & Other) const
{
	return Chars == Other.Chars;
}

TCHAR UMarkovChain::GetNext(FMarkovSource InSource)
{
	if (!MarkovMap.Contains(InSource))
		return TEXT('\0');

	TMap<TCHAR, int> Map = this->MarkovMap[InSource];

	int Total = 0;

	TArray<TCHAR> Keys;

	int KeyCount = Map.GetKeys(Keys);

	for (int i = 0; i < KeyCount; i++)
	{
		Total += Map[Keys[i]];
	}

	int Rng = this->Random.RandRange(0, Total - 1);

	for (int i = 0; i < KeyCount; i++)
	{
		Rng -= Map[Keys[i]];
		if (Rng < 0)
		{
			return Keys[i];
		}
	}

	return TEXT('\0');
}

FMarkovSource UMarkovChain::RandomSource()
{
	TArray<FMarkovSource> Keys;

	int KeyCount = this->MarkovMap.GetKeys(Keys);

	return Keys[Random.RandRange(0, KeyCount - 1)];
}

bool UMarkovChain::IsDeadEnd(FMarkovSource InSource, int Depth)
{
	if (Depth <= 0)
		return false;

	if (!MarkovMap.Contains(InSource))
		return true;

	TMap<TCHAR, int> Map = this->MarkovMap[InSource];

	TArray<TCHAR> Keys;

	if (Map.GetKeys(Keys) == 1)
		if(Keys[0] == TEXT('\0'))
			return true;

	FMarkovSource TempSource = InSource;

	for (int i = 0; i < Keys.Num(); ++i)
	{
		TempSource = InSource;
		TempSource.Rotate(Keys[i]);
		if (!IsDeadEnd(TempSource, Depth - 1)) return false;
	}

	return true;
}

TCHAR UMarkovChain::GetNextCharGuarantee(FMarkovSource InSource, int InSteps)
{
	check(!IsDeadEnd(InSource, InSteps));

	TMap<TCHAR, int> Temp;
	TMap<TCHAR, int> Map = MarkovMap[InSource];

	TArray<TCHAR> Keys;

	int KeyCount = Map.GetKeys(Keys);

	if (KeyCount == 1)
		return Keys[0];

	check(KeyCount > 0);

	int Total = 0;
	for (int i = 0; i < KeyCount; ++i)
	{
		FMarkovSource TempSource = InSource;
		TempSource.Rotate(Keys[i]);
		if (!IsDeadEnd(TempSource, InSteps))
		{
			if (Temp.Contains(Keys[i]))
				Temp[Keys[i]] = Map[Keys[i]];
			else
				Temp.Add(Keys[i], Map[Keys[i]]);
			Total += Map[Keys[i]];
		}
	}

	int Rng = Random.RandHelper(Total);

	TArray<TCHAR> TempKeys;

	int TempKeyCount = Temp.GetKeys(TempKeys);

	for (int i = 0; i < TempKeyCount; i++)
	{
		Rng -= Temp[TempKeys[i]];
		if (Rng < 0)
		{
			return TempKeys[i];
		}
	}

	check(Rng < 0);

	return TEXT('\0');
}

void UMarkovChain::Init(TArray<FString> InArray, int N, FRandomStream InRng)
{
	this->Random = InRng;

	for (FString ArrayString : InArray)
	{
		FMarkovSource Source;
		Source.SetCount(N);

		for (TCHAR Char : ArrayString)
		{
			if (Char == TEXT('\0'))
				break;

			if (!MarkovMap.Contains(Source))
				MarkovMap.Add(Source, TMap<TCHAR, int>());

			if (!MarkovMap[Source].Contains(Char))
				MarkovMap[Source].Add(Char, 0);

			MarkovMap[Source][Char]++;
			Source.Rotate(Char);
		}
		if (!MarkovMap.Contains(Source))
			MarkovMap.Add(Source, TMap<TCHAR, int>());

		if (!MarkovMap[Source].Contains(TEXT('\0')))
			MarkovMap[Source].Add(TEXT('\0'), 0);

		MarkovMap[Source][TEXT('\0')]++;
	}

	SourceCount = N;
}

FString UMarkovChain::GetMarkovString(int InLength)
{
	FString Out;

	FMarkovSource src;
	src.SetCount(SourceCount);

	if (InLength < 1) 
	{
		TCHAR tmp = GetNext(src);
		while (tmp != TEXT('\0'))
		{
			Out.AppendChar(tmp);
			src.Rotate(tmp);
			tmp = GetNext(src);
		}
		return Out;
	}
	for (int i = 0; i<InLength; ++i) {
		int x = (i > InLength - SourceCount ? InLength - i : SourceCount);
		TCHAR tmp = GetNextCharGuarantee(src, x);
		Out.AppendChar(tmp);
		src.Rotate(tmp);
	}
	return Out;
}

TArray<FString> UWorldGenerator::FilterTrainingData(TArray<UMarkovTrainingDataAsset*> InAssets, EMarkovTrainingDataUsage Usage)
{
	TArray<FString> Out;

	for (auto Asset : InAssets)
	{
		if (Asset->Usage == Usage)
		{
			Out.Append(Asset->TrainingData);
		}
	}

	return Out;
}

TArray<TCHAR> const& FMarkovSource::GetChars() const
{
	return Chars;
}

FString FMarkovSource::ToString() const
{
	FString Out;

	for (TCHAR Char : Chars)
		Out.AppendChar(Char);

	return Out;
}
