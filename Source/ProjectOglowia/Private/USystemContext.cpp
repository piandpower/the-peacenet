// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "USystemContext.h"
#include "PeacenetWorldStateActor.h"
#include "UDesktopWidget.h"
#include "UPeacegateFileSystem.h"
#include "CommonUtils.h"
#include "UAddressBookContext.h"
#include "UPeacegateProgramAsset.h"
#include "URainbowTable.h"
#include "UVulnerability.h"
#include "UVulnerabilityTerminalCommand.h"
#include "WallpaperAsset.h"
#include "ImageLoader.h"
#include "UGraphicalTerminalCommand.h"
#include "CommandInfo.h"

UAddressBookContext* USystemContext::GetAddressBook()
{
	check(this->Peacenet);

	

	if(!this->AddressBook)
	{
		this->AddressBook = NewObject<UAddressBookContext>(this);
		this->AddressBook->Setup(this);
	}

	return this->AddressBook;
}

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

	if (Peacenet->GameType->UnlockAllProgramsByDefault)
	{
		// Return ALL loaded programs if we're in a sandbox environment with all programs unlocked.
		return Peacenet->Programs;
	}

	TArray<UPeacegateProgramAsset*> OutArray;

	for (auto Executable : Computer.InstalledPrograms)
	{
		UPeacegateProgramAsset* FoundProgram;
		if (Peacenet->FindProgramByName(Executable, FoundProgram))
		{
			OutArray.Add(FoundProgram);
		}
	}

	return OutArray;
}

bool USystemContext::OpenProgram(FName InExecutableName, UProgram*& OutProgram)
{
	if (!Desktop)
	{
		return false;
	}

	for (auto Program : this->GetInstalledPrograms())
	{
		if (Program->ExecutableName == InExecutableName)
		{
			UWindow* MyWindow = nullptr;

			UProgram* MyProgram = UProgram::CreateProgram(Peacenet->WindowClass, Program->ProgramClass, this, Desktop->UserID, MyWindow);

			if (MyWindow && MyProgram)
			{
				MyWindow->Icon = Program->AppLauncherItem.Icon;
				MyWindow->WindowTitle = Program->AppLauncherItem.Name;
			}

			OutProgram = MyProgram;

			return true;
		}
	}

	return false;
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

	if (!(Computer.InstalledCommands.Contains(CommandName) || Computer.InstalledPrograms.Contains(CommandName)) && !Peacenet->GameType->UnlockAllProgramsByDefault)
		return false;

	if (!Peacenet->ManPages.Contains(CommandName))
		return false;

	FManPage ManPage = Peacenet->ManPages[CommandName];

	InternalUsage = ManPage.InternalUsage;
	FriendlyUsage = ManPage.FriendlyUsage;

	UPeacegateProgramAsset* Program = nullptr;
	if (Peacenet->FindProgramByName(CommandName, Program))
	{
		UGraphicalTerminalCommand* GraphicalCommand = NewObject<UGraphicalTerminalCommand>(this);
		GraphicalCommand->ProgramAsset = Program;
		GraphicalCommand->CommandInfo = Peacenet->CommandInfo[CommandName];
		OutCommand = GraphicalCommand;
		return true;
	}

	if (!Peacenet->CommandInfo.Contains(CommandName))
	{
		return false;
	}

	UCommandInfo* Info = Peacenet->CommandInfo[CommandName];
	OutCommand = NewObject<UTerminalCommand>(this, Info->Info.CommandClass);

	OutCommand->CommandInfo = Info;

	if (Info->IsA<UVulnerabilityCommandInfo>())
	{
		UVulnerability* Vuln = Cast<UVulnerabilityCommandInfo>(Info)->Vulnerability;
		Cast<UVulnerabilityTerminalCommand>(OutCommand)->Vulnerability = Vuln;
	}

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

	for (FUser User : Computer.Users)
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

void USystemContext::LogEvent(int UserID, FString Message)
{
	auto UserInfo = this->GetUserInfo(UserID);

	check(!UserInfo.Username.IsEmpty());

	float CurrentTimeOfDay = this->Peacenet->SaveGame->EpochTime;

	FEventLogEntry NewEntry;
	NewEntry.TimeOfDay = CurrentTimeOfDay;
	NewEntry.Username = UserInfo.Username;
	NewEntry.Message = Message;

	FString Serialized = UCommonUtils::ParseEventLogEntryToString(NewEntry);

	auto RootFS = this->GetFilesystem(0);

	FString LogText;
	EFilesystemStatusCode Anus;
	check(RootFS->ReadText("/var/log/peacegate.log", LogText, Anus));

	LogText.Append(Serialized + "\n");

	RootFS->WriteText("/var/log/peacegate.log", LogText);
}

void USystemContext::ShowWindowOnWorkspace(UProgram * InProgram)
{
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

	for (FUser User : Computer.Users)
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

	for (FUser User : Computer.Users)
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
	for (FUser User : Computer.Users)
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
	for (auto ProgramName : Computer.InstalledPrograms)
	{
		UPeacegateProgramAsset* Program;
		if (!Peacenet->FindProgramByName(ProgramName, Program))
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

void USystemContext::GetFolderTree(TArray<FFolder>& OutFolderTree)
{
	OutFolderTree = Computer.Filesystem;
}

void USystemContext::PushFolderTree(const TArray<FFolder>& InFolderTree)
{
	Computer.Filesystem = InFolderTree;
	if(Peacenet)
		Peacenet->UpdateComputer(Computer.ID, Computer, false);
}

FText USystemContext::GetTimeOfDay()
{
	return Peacenet->GetTimeOfDay();
}

void USystemContext::ParseCharacterName(const FString InCharacterName, FString & OutUsername, FString & OutHostname)
{
	// No sense doing this if there's only whitespace
	if (InCharacterName.IsEmpty())
		return;

	// Unix usernames can only be lower-case.
	FString NameString = InCharacterName.ToLower();

	// this will be the username.
	FString FirstName;
	FString Rem;

	// These characters are valid as name chars.
	const FString ValidUnixUsernameChars = TEXT("abcdefghijklmnopqrstuvwxyz0123456789_-");

	// the first char that isn't valid.
	TCHAR InvalidChar = TEXT('\0');

	// the chars in the name string
	TArray<TCHAR> NameChars = NameString.GetCharArray();

	for (auto Char : NameChars)
	{
		if (!ValidUnixUsernameChars.Contains(FString(1, &Char)))
		{
			InvalidChar = Char;
			break;
		}
	}

	// Did that for loop above change us?
	if (InvalidChar != TEXT('\0'))
	{
		NameString.Split(FString(1, &InvalidChar), &FirstName, &Rem);
	}
	else
	{
		FirstName = NameString;
	}

	OutUsername = FirstName;
	OutHostname = FirstName + TEXT("-pc");
}

void USystemContext::BroadcastNetMapEvent(int InEntityID, FNetMapScanEventArgs EventArgs)
{
	this->NetMapScan.Broadcast(InEntityID, EventArgs);
}

void USystemContext::ExecuteCommand(FString InCommand)
{
	check(this->Desktop);

	this->Desktop->ExecuteCommand(InCommand);
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

void USystemContext::UpdateSystemFiles()
{
	// This function updates the system based on save data and in-game assets.
	//
	// A.K.A: This is the function that updates things like what wallpapers are installed.

	// So first we need a root fs context.
	UPeacegateFileSystem* RootFS = this->GetFilesystem(0);

	// We're going to set up wallpapers.
	for (auto Wallpaper : this->Peacenet->Wallpapers)
	{
		// Is the wallpaper available? We don't care about removing those that aren't available but already exist on disk. We just want to update the ones that are unlocked.
		if (Wallpaper->UnlockedByDefault)
		{
			// Grab the texture's bitmap data.
			TArray<uint8> UETextureData = UImageLoader::GetBitmapData(Wallpaper->WallpaperTexture);

			// Now we just need to write it to disk.
			RootFS->WriteBinary(TEXT("/usr/share/wallpapers/") + Wallpaper->FriendlyName.ToString() + TEXT(".png"), UETextureData);
		}
	}

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