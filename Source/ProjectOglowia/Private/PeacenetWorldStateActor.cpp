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

	if (UGameplayStatics::DoesSaveGameExist(this->WorldSlot, 0))
	{
		SaveGame = Cast<UPeacenetSaveGame>(UGameplayStatics::LoadGameFromSlot(this->WorldSlot, 0));
	}
	else
	{
		SaveGame = NewObject<UPeacenetSaveGame>();

		FComputer Player;
		Player.ID = 0;
		Player.Hostname = FText::FromString(TEXT("player"));
		Player.ComputerType = this->PlayerComputerType->InternalID;
		Player.OwnerType = EComputerOwnerType::Player;

		FUser Root;
		Root.Username = FText::FromString(TEXT("root"));
		Root.Domain = EUserDomain::Administrator;
		Root.ID = 0;

		Player.Users.Add(Root);

		UFileUtilities::FormatFilesystem(Player.Filesystem);

		SaveGame->Computers.Add(Player);

		FRandomStream Rng = UWorldGenerator::GetRandomNumberGenerator(UWorldGenerator::GetSeedFromString(WorldSlot));

		UWorldGenerator::GenerateCharacters(SaveGame->Characters, SaveGame->Computers, Rng, TArray<FString>(), TArray<FString>());

		UGameplayStatics::SaveGameToSlot(SaveGame, WorldSlot, 0);
	}

	// Have we modified a PC?
	bool HasUpdatedPC = false;

	// Go through all the computers in the save.
	for (auto& PC : SaveGame->Computers)
	{
		// Is it a player?
		if (PC.OwnerType == EComputerOwnerType::Player)
		{
			// Go through all the programs in the game.
			for (auto Program : this->Programs)
			{
				// Is it unlocked by default?
				if (Program->IsUnlockedByDefault)
				{
					// Is it not installed on the NPC?
					if (!PC.InstalledPrograms.Contains(Program->ExecutableName))
					{
						// Add it.
						PC.InstalledPrograms.Add(Program->ExecutableName);
						HasUpdatedPC = true;
					}
				}
			}

			// Go through every loaded command.
			TArray<FName> CommandNames;
			CommandInfo.GetKeys(CommandNames);

			for (auto CommandName : CommandNames)
			{
				UCommandInfo* LoadedCommand = CommandInfo[CommandName];
				if (LoadedCommand)
				{
					if (LoadedCommand->UnlockedByDefault)
					{
						if (!PC.InstalledCommands.Contains(CommandName))
						{
							PC.InstalledCommands.Add(CommandName);
							HasUpdatedPC = true;
						}
					}
				}
			}

		}
	}


	// Save the game if a PC was updated
	if (HasUpdatedPC)
	{
		UGameplayStatics::SaveGameToSlot(SaveGame, WorldSlot, 0);
	}
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
