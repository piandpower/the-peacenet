// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "USystemContext.h"
#include "Kismet/GameplayStatics.h"
#include "PeacenetWorldStateActor.h"
#include "UDesktopWidget.h"
#include "UPeacegateFileSystem.h"
#include "CommonUtils.h"
#include "UPeacegateProgramAsset.h"
#include "UUserContext.h"
#include "UProgram.h"
#include "URainbowTable.h"
#include "WallpaperAsset.h"
#include "UGraphicalTerminalCommand.h"
#include "CommandInfo.h"

FString ReadFirstLine(FString InText)
{
	if (InText.Contains("\n"))
	{
		int NewLineIndex;
		InText.FindChar('\n', NewLineIndex);
		return InText.Left(NewLineIndex).TrimStartAndEnd();
	}
	else
	{
		return InText.TrimStartAndEnd();
	}
}

FString USystemContext::GetHostname()
{
	if (!CurrentHostname.IsEmpty())
	{
		// Speed increase: No need to consult the filesystem for this.
		return CurrentHostname;
	}
	
	UPeacegateFileSystem* RootFS = this->GetFilesystem(0);
	if (RootFS->FileExists("/etc/hostname"))
	{
		EFilesystemStatusCode StatusCode;
		RootFS->ReadText("/etc/hostname", this->CurrentHostname, StatusCode);
		CurrentHostname = ReadFirstLine(CurrentHostname);
		return this->CurrentHostname;
	}

	CurrentHostname = "localhost";
	return CurrentHostname;
}

TArray<UPeacegateProgramAsset*> USystemContext::GetInstalledPrograms()
{
	check(Peacenet);

	if (!GetPeacenet()->GameType->GameRules.DoUnlockables)
	{
		// Return ALL loaded programs if we're in a sandbox environment with all programs unlocked.
		return GetPeacenet()->Programs;
	}

	TArray<UPeacegateProgramAsset*> OutArray;

	for (auto Executable : GetComputer().InstalledPrograms)
	{
		UPeacegateProgramAsset* FoundProgram;
		if (GetPeacenet()->FindProgramByName(Executable, FoundProgram))
		{
			OutArray.Add(FoundProgram);
		}
	}

	return OutArray;
}

bool USystemContext::OpenProgram(FName InExecutableName, UProgram*& OutProgram, bool InCheckForExistingWindow)
{
	check(this->GetPeacenet());
	check(this->GetDesktop());

	UPeacegateProgramAsset* PeacegateProgram = nullptr;

	if(!this->GetPeacenet()->FindProgramByName(InExecutableName, PeacegateProgram))
		return false;

	if(this->GetPeacenet()->GameType->GameRules.DoUnlockables)
	{
		if(!this->GetComputer().InstalledPrograms.Contains(InExecutableName) && !PeacegateProgram->IsUnlockedByDefault)
		{
			return false;
		}
	}

	UProgram* Program = this->GetDesktop()->SpawnProgramFromClass(PeacegateProgram->ProgramClass, PeacegateProgram->AppLauncherItem.Name, PeacegateProgram->AppLauncherItem.Icon);

	check(Program);

	this->GetDesktop()->ShowProgramOnWorkspace(Program);

	return Program;
}

UPeacegateFileSystem * USystemContext::GetFilesystem(const int UserID)
{
	if (!RegisteredFilesystems.Contains(UserID))
	{
		UPeacegateFileSystem* NewFS = UCommonUtils::CreateFilesystem(this, UserID);
		TScriptDelegate<> ModifiedDelegate;
		ModifiedDelegate.BindUFunction(this, "HandleFileSystemEvent");
		NewFS->FilesystemOperation.Add(ModifiedDelegate);
		this->RegisteredFilesystems.Add(UserID, NewFS);
		return NewFS;
	}

	return this->RegisteredFilesystems[UserID];
}

bool USystemContext::TryGetTerminalCommand(FName CommandName, UTerminalCommand *& OutCommand, FString& InternalUsage, FString& FriendlyUsage)
{
	check(Peacenet);

	if (!(GetComputer().InstalledCommands.Contains(CommandName) || GetComputer().InstalledPrograms.Contains(CommandName)) && Peacenet->GameType->GameRules.DoUnlockables)
		return false;

	if (!GetPeacenet()->ManPages.Contains(CommandName))
		return false;

	FManPage ManPage = GetPeacenet()->ManPages[CommandName];

	InternalUsage = ManPage.InternalUsage;
	FriendlyUsage = ManPage.FriendlyUsage;

	UPeacegateProgramAsset* Program = nullptr;
	if (GetPeacenet()->FindProgramByName(CommandName, Program))
	{
		UGraphicalTerminalCommand* GraphicalCommand = NewObject<UGraphicalTerminalCommand>(this);
		GraphicalCommand->ProgramAsset = Program;
		GraphicalCommand->CommandInfo = Peacenet->CommandInfo[CommandName];
		OutCommand = GraphicalCommand;
		return true;
	}

	if (!GetPeacenet()->CommandInfo.Contains(CommandName))
	{
		return false;
	}

	UCommandInfo* Info = GetPeacenet()->CommandInfo[CommandName];
	OutCommand = NewObject<UTerminalCommand>(this, Info->Info.CommandClass);

	OutCommand->CommandInfo = Info;

	return true;
}

FUserInfo USystemContext::GetUserInfo(const int InUserID)
{
	if (InUserID == -1)
	{
		FUserInfo AnonInfo;
		AnonInfo.IsAdminUser = false;
		AnonInfo.Username = "<anonymous>";
		return AnonInfo;
	}

	for (FUser User : GetComputer().Users)
	{
		if (User.ID == InUserID)
		{
			FUserInfo Info;
			Info.Username = User.Username;
			Info.IsAdminUser = (User.Domain == EUserDomain::Administrator);
			return Info;
		}
	}

	return FUserInfo();
}

void USystemContext::ShowWindowOnWorkspace(UProgram * InProgram)
{
	// DEPRECATED IN FAVOUR OF UUserContext::ShowProgramOnWorkspace().
	if (Desktop && InProgram)
	{
		Desktop->ShowProgramOnWorkspace(InProgram);
	}
}

EUserDomain USystemContext::GetUserDomain(int InUserID)
{
	if (InUserID == -1)
	{
		return EUserDomain::Anonymous;
	}

	for (FUser User : GetComputer().Users)
	{
		if (User.ID == InUserID)
		{
			return User.Domain;
		}
	}

	return EUserDomain::User;
}

FString USystemContext::GetUsername(int InUserID)
{
	FUserInfo UserInfo = this->GetUserInfo(InUserID);
	return UserInfo.Username;
}

FString USystemContext::GetUserHomeDirectory(int UserID)
{
	if (this->GetUserDomain(UserID) == EUserDomain::Anonymous)
	{
		return "/";
	}

	for (FUser User : GetComputer().Users)
	{
		if (User.ID == UserID)
		{
			if (User.Domain == EUserDomain::Administrator)
				return TEXT("/root");
			return TEXT("/home/") + User.Username;
		}
	}

	return FString();
}

bool USystemContext::Authenticate(const FString & Username, const FString & Password, int & UserID)
{
	for (FUser User : GetComputer().Users)
	{
		if (User.Username == Username && User.Password == Password)
		{
			UserID = User.ID;
			return true;
		}
	}

	return false;
}

bool USystemContext::GetSuitableProgramForFileExtension(const FString & InExtension, UPeacegateProgramAsset *& OutProgram)
{
	for (auto ProgramName : GetComputer().InstalledPrograms)
	{
		UPeacegateProgramAsset* Program;
		if (!GetPeacenet()->FindProgramByName(ProgramName, Program))
		{
			continue;
		}
		if (Program->SupportedFileExtensions.Contains(InExtension))
		{
			OutProgram = Program;
			return true;
		}
	}
	return false;
}

UDesktopWidget* USystemContext::GetDesktop()
{
	return this->Desktop;
}

FPeacenetIdentity& USystemContext::GetCharacter()
{
	check(this->GetPeacenet());

	auto MyPeacenet = this->GetPeacenet();

	int CharacterIndex = 0;
	FPeacenetIdentity Character;

	check(MyPeacenet->SaveGame->GetCharacterByID(this->CharacterID, Character, CharacterIndex));

	return MyPeacenet->SaveGame->Characters[CharacterIndex];
}

UUserContext* USystemContext::GetUserContext(int InUserID)
{
	if(this->Users.Contains(InUserID))
	{
		return this->Users[InUserID];
	}
	else
	{
		UUserContext* User = NewObject<UUserContext>(this);
		User->Setup(this, InUserID);
		Users.Add(InUserID, User);
		return User;
	}
}

FComputer& USystemContext::GetComputer()
{
	check(this->GetPeacenet());

	auto MyPeacenet = this->GetPeacenet();

	int ComputerIndex = 0;
	FComputer Computer;

	check(MyPeacenet->SaveGame->GetComputerByID(this->ComputerID, Computer, ComputerIndex));

	return MyPeacenet->SaveGame->Computers[ComputerIndex];
}

APeacenetWorldStateActor* USystemContext::GetPeacenet()
{
	return this->Peacenet;
}

URainbowTable* USystemContext::GetRainbowTable()
{
	return this->RainbowTable;
}

void USystemContext::SetupDesktop(int InUserID)
{
	check(!this->GetDesktop());

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetPeacenet()->GetWorld(), 0);

	this->Desktop = CreateWidget<UDesktopWidget, APlayerController>(PlayerController, this->GetPeacenet()->DesktopClass);

	check(GetDesktop());

	this->Desktop->SystemContext = this;
	this->Desktop->UserID = InUserID;
}

void USystemContext::GetFolderTree(TArray<FFolder>& OutFolderTree)
{
	OutFolderTree = GetComputer().Filesystem;
}

void USystemContext::PushFolderTree(const TArray<FFolder>& InFolderTree)
{
	GetComputer().Filesystem = InFolderTree;
}

FText USystemContext::GetTimeOfDay()
{
	return GetPeacenet()->GetTimeOfDay();
}

void USystemContext::ExecuteCommand(FString InCommand)
{
	check(this->GetDesktop());

	this->GetDesktop()->ExecuteCommand(InCommand);
}

void USystemContext::HandleFileSystemEvent(EFilesystemEventType InType, FString InPath)
{
	switch (InType)
	{
	case EFilesystemEventType::WriteFile:
		if (InPath == "/etc/hostname")
		{
			auto fs = GetFilesystem(0);
			EFilesystemStatusCode err;
			fs->ReadText("/etc/hostname", this->CurrentHostname, err);
			CurrentHostname = ReadFirstLine(CurrentHostname);
		}
		break;
	}

	// If the path is within /var we might want to check to make sure the log still exists.
	if (InPath.StartsWith("/var"))
	{
		auto RootFS = GetFilesystem(0);

		EFilesystemStatusCode Anus;

		// Does /var/log not exist?
		if (!RootFS->DirectoryExists("/var/log"))
		{
			if (!RootFS->DirectoryExists("/var"))
			{
				RootFS->CreateDirectory("/var", Anus);
			}
			RootFS->CreateDirectory("/var/log", Anus);
		}

		// Does peacegate.log not exist?
		if (!RootFS->FileExists("/var/log/peacegate.log"))
		{
			// write blank log.
			RootFS->WriteText("/var/log/peacegate.log", "");
		}

	}
}

TArray<UWallpaperAsset*> USystemContext::GetAvailableWallpapers()
{
	TArray<UWallpaperAsset*> Ret;
	for (auto Wallpaper : this->GetPeacenet()->Wallpapers)
	{
		if(Wallpaper->IsDefault || Wallpaper->UnlockedByDefault || this->GetComputer().UnlockedWallpapers.Contains(Wallpaper->InternalID))
		{
			Ret.Add(Wallpaper);
		}
	}
	return Ret;
}

void USystemContext::SetCurrentWallpaper(UWallpaperAsset* InWallpaperAsset)
{
	// Make sure it's not null.
	check(InWallpaperAsset);

	// If it's unlocked by default or already unlocked, we just set it.
	if(InWallpaperAsset->UnlockedByDefault || InWallpaperAsset->IsDefault || this->GetComputer().UnlockedWallpapers.Contains(InWallpaperAsset->InternalID))
	{
		// Set the wallpaper.
		this->GetComputer().CurrentWallpaper = InWallpaperAsset->WallpaperTexture;
	}
	else
	{
		// BETA TODO: Announce item unlock.
		this->GetComputer().UnlockedWallpapers.Add(InWallpaperAsset->InternalID);

		// Set the wallpaper.
		this->GetComputer().CurrentWallpaper = InWallpaperAsset->WallpaperTexture;
	}
}

void USystemContext::UpdateSystemFiles()
{
	// This function updates the system based on save data and in-game assets.
	//
	// A.K.A: This is the function that updates things like what wallpapers are installed.

	// So first we need a root fs context.
	UPeacegateFileSystem* RootFS = this->GetFilesystem(0);

	EFilesystemStatusCode Anus;

	// Does /var/log not exist?
	if (!RootFS->DirectoryExists("/var/log"))
	{
		if (!RootFS->DirectoryExists("/var"))
		{
			RootFS->CreateDirectory("/var", Anus);
		}
		RootFS->CreateDirectory("/var/log", Anus);
	}

	// Does peacegate.log not exist?
	if (!RootFS->FileExists("/var/log/peacegate.log"))
	{
		// write blank log.
		RootFS->WriteText("/var/log/peacegate.log", "");
	}

	// This is also where we init our rainbow table.
	this->RainbowTable = NewObject<URainbowTable>(this);
	this->RainbowTable->Setup(this, "/etc/rainbow_table.db", true);
}

void USystemContext::Setup(int InComputerID, int InCharacterID, APeacenetWorldStateActor* InPeacenet)
{
	check(InPeacenet);

	this->ComputerID = InComputerID;
	this->CharacterID = InCharacterID;
	this->Peacenet = InPeacenet;
}