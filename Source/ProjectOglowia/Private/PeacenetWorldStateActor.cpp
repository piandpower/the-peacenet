// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "PeacenetWorldStateActor.h"
#include "UWorldGenerator.h"
#include "Kismet/GameplayStatics.h"
#include "UComputerTypeAsset.h"
#include "UPeacegateProgramAsset.h"
#include "CommandInfo.h"
#include "TerminalCommand.h"
#include "AssetRegistry/Public/IAssetRegistry.h"
#include "AssetRegistry/Public/AssetRegistryModule.h"
#include "UWindow.h"

bool APeacenetWorldStateActor::UpdateComputer(int InEntityID, FComputer & InComputer)
{
	if (!SaveGame)
	{
		return false;
	}

	for (int i = 0; i < SaveGame->Computers.Num(); i++)
	{
		FComputer OldPC = SaveGame->Computers[i];
		if (OldPC.ID == InEntityID)
		{
			SaveGame->Computers[i] = InComputer;
			if (InComputer.OwnerType == EComputerOwnerType::Player)
			{
				UGameplayStatics::SaveGameToSlot(SaveGame, WorldSlot, 0);
			}
			return true;
		}
	}

	return false;
}

// Sets default values
APeacenetWorldStateActor::APeacenetWorldStateActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Loads all the terminal commands in the game
void APeacenetWorldStateActor::LoadTerminalCommands()
{
	TArray<UCommandInfo*> Commands;

	// Load command assets.
	this->LoadAssets(TEXT("CommandInfo"), Commands);

	// Help command is not processed in blueprint land.
	UCommandInfo* HelpInfo = NewObject<UCommandInfo>(this);
	HelpInfo->Info.CommandName = TEXT("help");
	HelpInfo->Info.Description = TEXT("Shows a list of commands you can run.");
	HelpInfo->Info.CommandClass = TSubclassOf<UTerminalCommand>(UHelpCommand::StaticClass());
	HelpInfo->UnlockedByDefault = true;
	Commands.Add(HelpInfo);

	for (auto Command : Commands)
	{
		this->CommandInfo.Add(Command->Info.CommandName, Command);
	
		// Compile the command info into Docopt usage strings
		FString UsageFriendly = UCommandInfo::BuildDisplayUsageString(Command->Info);
		FString UsageInternal = UCommandInfo::BuildInternalUsageString(Command->Info);

		// Create a manual page for the command
		FManPage ManPage;
		ManPage.Description = Command->Info.Description;
		ManPage.InternalUsage = UsageInternal;
		ManPage.FriendlyUsage = UsageFriendly;

		// Save the manual page for later use
		ManPages.Add(Command->Info.CommandName, ManPage);
	}


}

// Called when the game starts or when spawned
void APeacenetWorldStateActor::BeginPlay()
{
	Super::BeginPlay();

	// Load all the game's programs
	LoadAssets(TEXT("PeacegateProgramAsset"), this->Programs);

	// Load terminal command assets, build usage strings, populate command map.
	this->LoadTerminalCommands();
}

// Called every frame
void APeacenetWorldStateActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Is the save loaded?
	if (SaveGame)
	{
		// Get time of day
		float TimeOfDay = SaveGame->EpochTime;

		// Tick it.
		TimeOfDay += (DeltaTime * 6);

		// Has it been a full day yet?
		if (TimeOfDay >= SaveGame->SECONDS_DAY_LENGTH)
		{
			TimeOfDay = 0;
		}

		// Save it
		SaveGame->EpochTime = TimeOfDay;
	}
}

void APeacenetWorldStateActor::StartGame()
{
	FComputer PlayerPC = SaveGame->Computers[0];

	USystemContext* PlayerContext = NewObject<USystemContext>();

	PlayerContext->Computer = PlayerPC;
	PlayerContext->Peacenet = this;

	UWorldGenerator::GenerateSystemDirectories(PlayerContext);
	
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	PlayerContext->Desktop = CreateWidget<UDesktopWidget, APlayerController>(PlayerController, this->DesktopClass);

	PlayerContext->Desktop->SystemContext = PlayerContext;

	SystemContexts.Add(PlayerContext);


	PlayerSystemReady.Broadcast(PlayerContext);
}

bool APeacenetWorldStateActor::FindProgramByName(FName InName, UPeacegateProgramAsset *& OutProgram)
{
	for (auto Program : this->Programs)
	{
		if (Program->ExecutableName == InName)
		{
			OutProgram = Program;
			return true;
		}
	}

	return false;
}

FText APeacenetWorldStateActor::GetTimeOfDay()
{
	if (!SaveGame)
		return FText();

	float TOD = SaveGame->EpochTime;

	float Seconds = FMath::Fmod(TOD, 60.f);
	float Minutes = FMath::Fmod(TOD / 60.f, 60.f);
	float Hours = FMath::Fmod((TOD / 60.f) / 60.f, 24.f);

	FString MinutesString = FString::FromInt((int)Minutes);
	if (Minutes < 10.f)
	{
		MinutesString = TEXT("0") + MinutesString;
	}

	FString HoursString = FString::FromInt((int)Hours);

	return FText::FromString(HoursString + TEXT(":") + MinutesString);
}

template<typename AssetType>
bool APeacenetWorldStateActor::LoadAssets(FName ClassName, TArray<AssetType*>& OutArray)
{
	// Get the Asset Registry
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

	// A place to store computer type asset data
	TArray<FAssetData> Assets;

	if (!AssetRegistryModule.Get().GetAssetsByClass(ClassName, Assets, true))
		return false;

	for (auto& Asset : Assets)
	{
		OutArray.Add((AssetType*)Asset.GetAsset());
	}

	return true;
}

bool APeacenetWorldStateActor::HasExistingOS()
{
	return UGameplayStatics::DoesSaveGameExist(TEXT("PeacegateOS"), 0);
}

APeacenetWorldStateActor* APeacenetWorldStateActor::GenerateAndCreateWorld(const APlayerController* InPlayerController, const FPeacenetWorldInfo& InWorldInfo)
{
	// This function is responsible for initially generating a Peacenet world.
	// We take in an FPeacenetWorldInfo structure by-ref so we know how to spawn
	// the player's computer and Peacenet identity.
	//
	// We take in a Player Controller so that we have a world context object.
	// Eventually, we won't take in a raw UE4 Player Controller - we'll take in
	// our own Player Controller, allowing us to directly call back into
	// the game's Blueprint land, automatically telling the controller
	// to possess a new Peacegate OS pawn when the world generator finishes.

	// But, that can wait until I've had my fucking coffee... on Boxing Day of 2025.
	// - Alkaline

	// For now, get the current world:
	UWorld* CurrentWorld = InPlayerController->GetWorld();

	// For now, that's all we need that player controller for. We can now spawn the Peacenet world.
	auto NewPeacenet = CurrentWorld->SpawnActor<APeacenetWorldStateActor>();

	// Now we can create a world seed. The world seed will be generated from the combined string of the player's full name, username and hostname. This creates a unique world for each player.
	FString CombinedPlayerName = InWorldInfo.PlayerName.ToString() + TEXT("_") + InWorldInfo.PlayerUsername.ToString() + TEXT("_") + InWorldInfo.PlayerHostname.ToString();

	// We can use a CRC memory hash function to create the seed.
	TArray<TCHAR> SeedChars = CombinedPlayerName.GetCharArray();
	int Seed = FCrc::MemCrc32(SeedChars.GetData(), sizeof(TCHAR) * SeedChars.Num());

	// Suitable seed for a random number generator. So, let Peacenet create one with it.
	FRandomStream WorldGenerator = UWorldGenerator::GetRandomNumberGenerator(Seed);

	// Create a new computer for the player.
	FComputer PlayerComputer;

	// Set its entity metadata.
	PlayerComputer.ID = 0;
	PlayerComputer.ComputerType = TEXT("c_desktop"); // TODO: shouldn't be hardcoded.
	PlayerComputer.Hostname = InWorldInfo.PlayerHostname;
	PlayerComputer.OwnerType = EComputerOwnerType::Player;

	// Create a unix root user.
	FUser RootUser;
	RootUser.ID = 0;
	RootUser.Username = FText::FromString(TEXT("root"));
	RootUser.Password = FText::FromString(TEXT(""));
	RootUser.Domain = EUserDomain::Administrator;

	// Create a non-root user.
	FUser PlayerUser;
	PlayerUser.ID = 1;
	PlayerUser.Username = InWorldInfo.PlayerUsername;
	PlayerUser.Password = InWorldInfo.PlayerPassword;
	PlayerUser.Domain = EUserDomain::PowerUser;

	// Assign users to the new computer.
	PlayerComputer.Users.Add(RootUser);
	PlayerComputer.Users.Add(PlayerUser);

	// World generator can generate the computer's filesystem.
	UWorldGenerator::CreateFilesystem(PlayerComputer, WorldGenerator);

	// Note: The save file would have been loaded as soon as the actor spawned - BeginPlay gets called during UWorld::SpawnActor.
	// So, at this point, we've had a save file created and ready for us for a few CPU cycles now...

	// Give our new Peacenet back to the Blueprint land or whoever else happened to call us.
	return NewPeacenet;
}