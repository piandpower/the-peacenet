// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UWorldGenerator.h"
#include "UComputerTypeAsset.h"
#include "AssetRegistryModule.h"
#include "USystemContext.h"
#include "UPeacenetSaveGame.h"
#include "FEnterpriseNetwork.h"
#include "UPeacegateFileSystem.h"

FString UWorldGenerator::GenerateRandomName(const FRandomStream& InGenerator, const TArray<FString> InFirstNames, TArray<FString> InLastNames)
{
	FString first = InFirstNames[InGenerator.RandRange(0, InFirstNames.Num() - 1)];
	FString last = InLastNames[InGenerator.RandRange(0, InLastNames.Num() - 1)];

	return first + TEXT(" ") + last;
}

void UWorldGenerator::GenerateCharacters(const FRandomStream & InRandomStream, UPeacenetSaveGame * InSaveGame)
{
	// This is a list of all training data assets in the game.
	TArray<UMarkovTrainingDataAsset*> TrainingData;

	// The UE4 asset data needed for that above array.
	TArray<FAssetData> TrainingDataAssets;

	// We need to load in the assets into that above array.
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	check(AssetRegistryModule.Get().GetAssetsByClass(TEXT("MarkovTrainingDataAsset"), TrainingDataAssets, true));
	for (FAssetData Asset : TrainingDataAssets)
	{
		TrainingData.Add(Cast<UMarkovTrainingDataAsset>(Asset.GetAsset()));
	}

	// These arrays contain aggregated training data for Markov chains.
	TArray<FString> MaleFirstNames = FilterTrainingData(TrainingData, EMarkovTrainingDataUsage::MaleFirstNames);
	TArray<FString> FemaleFirstNames = FilterTrainingData(TrainingData, EMarkovTrainingDataUsage::FemaleFirstNames);
	TArray<FString> BusinessWords = FilterTrainingData(TrainingData, EMarkovTrainingDataUsage::Hostnames);
	TArray<FString> LastNames = FilterTrainingData(TrainingData, EMarkovTrainingDataUsage::LastNames);

	// Create markov chains for first names.
	UMarkovChain* MaleGenerator = CreateMarkovChain(MaleFirstNames, InRandomStream);
	UMarkovChain* FemaleGenerator = CreateMarkovChain(FemaleFirstNames, InRandomStream);


	// And one for the last names.
	UMarkovChain* LastNameGenerator = UWorldGenerator::CreateMarkovChain(LastNames, InRandomStream);

	// I want to generate a lot of NPCs, so I'm going to loop through all the countries.
	for (int i = 0; i < (int)ECountry::Num_Countries; i++)
	{
		// All NPCs spawned below will spawn here.
		ECountry Country = (ECountry)i;

		// These values don't indicate how many npcs/business exactly that we'll generate.
		// The amount varies from world seed to world seed. The variation comes from the fact that
		// the relevant value is decreased by a random number between 1 and 5 every time
		// an NPC or business is generated.
		//
		// When the relevant value reaches zero or below, the game starts generating the next
		// part of the world.
		int NPCCounter = 1000;
		int BusinessCounter = 400;


		// Now we have all our training data for name generation.
		// So, now we'll start generating NPCs.
		while (NPCCounter > 0)
		{
			// Allocate memory for a new character.
			FPeacenetIdentity NPC;

			// This NPC's entity ID becomes the number of entities in the world.
			NPC.ID = InSaveGame->Characters.Num();

			// Now we get to generate their name.

			// To do this, we need to know their gender. Sorry, 21st century, but Peacenet's world generator only recognizes males and females as genders.
			bool IsMale = (InRandomStream.RandRange(1, 6) % 2) == 0; // odd = female, even = male. No, I'm not being sexist, just adding a 50% chance. Females aren't odd. They just spawn in the game when the dice rolls an odd number. Okay?

			// Pick the right name generator for the gender.
			UMarkovChain* FirstNameGenerator = (IsMale ? MaleGenerator : FemaleGenerator);

			// We do this in a loop until we come up with a character name that is NOT taken yet.
			do
			{
				// Generate the first and last name of the NPC.
				FString FirstName = MakeName(FirstNameGenerator->GetMarkovString(0));
				FString LastName = MakeName(LastNameGenerator->GetMarkovString(0));

				// Combine it into a single Text variable as the NPC's full name.
				NPC.CharacterName = FText::FromString(FirstName + TEXT(" ") + LastName);
			} while (InSaveGame->CharacterNameExists(NPC.CharacterName));

			// The NPC is not a player or story character.
			NPC.CharacterType = EIdentityType::NonPlayer;

			// This is the character's raw reputation value.
			float RawReputation = InRandomStream.GetFraction();

			// Roll a dice, if it's odd, the NPC becomes a malicious one.
			if (InRandomStream.RandRange(1, 6) % 2 != 0)
			{
				RawReputation = -RawReputation;
			}

			// RawReputation is now the true reputation value.
			NPC.Reputation = RawReputation;

			// Now we set the country.
			NPC.Country = Country;

			// TODO: Skill generation.
			NPC.Skill = 1;

			// Add the character to the save!
			InSaveGame->Characters.Add(NPC);

			// Decrease counter.
			NPCCounter -= InRandomStream.RandRange(1, 5);
		}

		while (BusinessCounter > 0)
		{
			FEnterpriseNetwork Business;

			// Set the ID of the network.
			Business.ID = InSaveGame->Businesses.Num();
		
			// TODO: business names.
			Business.Name = FText::FromString(TEXT("Business Inc."));

			// Set the country.
			Business.Country = Country;

			InSaveGame->Businesses.Add(Business);

			BusinessCounter -= InRandomStream.RandRange(1, 5);
		}

		// Now we generate each NPC's computer.
		for (auto& NPC : InSaveGame->Characters)
		{
			// Allocate memory for the new computer.
			FComputer Computer;

			// set the ID just like a character.
			Computer.ID = InSaveGame->Computers.Num();

			// Update the NPC's computer ID to match.
			NPC.ComputerID = Computer.ID;

			// These values are crucial for the computer.
			FString Username;
			FString Hostname;
			FString Password;
			FString RootPassword;

			// The password length is equal to rand(3, 5) * (2 ^ skill).
			int PasswordLength = InRandomStream.RandRange(3, 5) * FMath::Pow(2, NPC.Skill);

			// Hostname and user generation is really easy, since this is a personal computer.
			USystemContext::ParseCharacterName(NPC.CharacterName.ToString(), Username, Hostname);

			// For the password, I probably have a function for that. But who knows?
			Password = GenerateRandomPassword(InRandomStream, PasswordLength);

			// Whether or not we have a root password is based on a random number between 1 and (4 ^ Skill) being less than 2.
			if (InRandomStream.RandRange(1, FMath::Pow(4, NPC.Skill)) > 2)
			{
				// Anything above 2 means we get a root password.
				RootPassword = GenerateRandomPassword(InRandomStream, PasswordLength);
			}

			// Now that we have the user info... we can assign it all to the computer.
			Computer.Hostname = FText::FromString(Hostname);

			// Create the root user:
			FUser Root;
			Root.Username = FText::FromString(TEXT("root"));
			Root.Password = FText::FromString(RootPassword);
			Root.Domain = EUserDomain::Administrator;

			// Create the NPC user:
			FUser NonRoot;
			NonRoot.Username = FText::FromString(Username);
			NonRoot.Password = FText::FromString(Password);
			NonRoot.Domain = EUserDomain::PowerUser;

			// Set their uids.
			Root.ID = 0;
			NonRoot.ID = 1;

			// Add them to the computer.
			Computer.Users.Add(Root);
			Computer.Users.Add(NonRoot);

			// Generate the filesystem!!
			UWorldGenerator::CreateFilesystem(Computer, InRandomStream);

			// And add the computer to the world.
			InSaveGame->Computers.Add(Computer);
		}
	}
}

FString UWorldGenerator::MakeName(FString InWord)
{
	FString OutName;
	for (int i = 0; i < InWord.GetCharArray().Num()-1; i++)
	{
		if (i == 0)
		{
			OutName.Append(FString::Chr(InWord[i]).ToUpper());
		}
		else
		{
			OutName.AppendChar(InWord[i]);
		}
	}
	return OutName;
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
	Filesystem->CreateDirectory(TEXT("/home"), InstallStatusCode);
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
	for (i = 0; i < Chars.Num() - 1; i++)
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

		for (TCHAR Char : ArrayString.ToLower())
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

void UWorldGenerator::CreateFilesystem(FComputer& InComputer, const FRandomStream& InGenerator)
{
	// Format the filesystem.
	UFileUtilities::FormatFilesystem(InComputer.Filesystem);

	// Create a system context.
	USystemContext* SysCtx = NewObject<USystemContext>();
	SysCtx->Computer = InComputer;
	
	// Creates system directories for the computer. This should include all users' home directories.
	UWorldGenerator::GenerateSystemDirectories(SysCtx);

	// At this point, we would also generate lootable files. However, that isn't implemented into the game yet.

	// This system context isn't actually registered with a world context, so it can't update the save file.
	// That's why we took in our computer by-reference, so we can report back the changes to the calling function.
	InComputer = SysCtx->Computer;
}
