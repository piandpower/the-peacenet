// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UWorldGenerator.h"
#include "UComputerTypeAsset.h"
#include "AssetRegistryModule.h"
#include "UNonPlayerSystemContext.h"

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

	// The class of a computer type asset
	UClass* ComputerTypeClass = UComputerTypeAsset::StaticClass();

	AssetRegistryModule.Get().GetAssetsByClass(TEXT("ComputerTypeAsset"), ComputerTypeAssets, true);

	TArray<UComputerTypeAsset*> ComputerTypes;

	// Load all the computer type assets, we'll need them for later.
	for (auto& Asset : ComputerTypeAssets)
	{
		ComputerTypes.Add((UComputerTypeAsset*)Asset.GetAsset());
	}

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
			NewComputer.Hostname = FText::FromString(TEXT("npc_host_") + FString::FromInt(NewComputer.ID));

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
			UNonPlayerSystemContext* SysCtx = NewObject<UNonPlayerSystemContext>();
			SysCtx->Computer = NewComputer;

// This lets us get a filesystem context as if we were logged into Peacegate OS as the root user we created above.
// We're still generating the computer - there's no Peacegate yet - we're about to "install" it.
// However, since we're in the C++ land, we can create a Peacegate context before Peacegate exists so we can generate the system properly.

			UPeacegateFileSystem* Filesystem = SysCtx->GetFilesystem_Implementation(RootUser.ID);

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
for (auto User : NewComputer.Users)
{
	FString Home = SysCtx->GetUserHomeDirectory(User.ID);
				
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