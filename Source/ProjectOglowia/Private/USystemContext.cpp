// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "USystemContext.h"
#include "PeacenetWorldStateActor.h"
#include "UDesktopWidget.h"
#include "UPeacegateFileSystem.h"
#include "CommonUtils.h"
#include "UPeacegateProgramAsset.h"
#include "UGraphicalTerminalCommand.h"
#include "CommandInfo.h"

FString USystemContext::GetHostname() const
{
	return Computer.Hostname.ToString();
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

bool USystemContext::OpenProgram(FName InExecutableName)
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

			UProgram* MyProgram = UProgram::CreateProgram(Peacenet, Peacenet->WindowClass, Program->ProgramClass, this, Desktop->UserID, MyWindow);

			if (MyWindow && MyProgram)
			{
				MyWindow->Icon = Program->AppLauncherItem.Icon;
				MyWindow->WindowTitle = FText::FromString(Program->AppLauncherItem.Name);
			}

			return true;
		}
	}

	return false;
}

UPeacegateFileSystem * USystemContext::GetFilesystem(const int UserID)
{
	return UCommonUtils::CreateFilesystem(this, UserID);
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
		OutCommand = GraphicalCommand;
		return true;
	}

	if (!Peacenet->CommandInfo.Contains(CommandName))
	{
		return false;
	}

	UCommandInfo* Info = Peacenet->CommandInfo[CommandName];
	OutCommand = NewObject<UTerminalCommand>(this, Info->Info.CommandClass);

	return true;
}

FUserInfo USystemContext::GetUserInfo(const int InUserID)
{
	for (FUser User : Computer.Users)
	{
		if (User.ID == InUserID)
		{
			FUserInfo Info;
			Info.Username = User.Username.ToString();
			Info.IsAdminUser = (User.Domain == EUserDomain::Administrator);
			return Info;
		}
	}

	return FUserInfo();
}

void USystemContext::ShowWindowOnWorkspace(const UProgram * InProgram)
{
	if (Desktop && InProgram)
	{
		Desktop->ShowProgramOnWorkspace(InProgram);
	}
}

EUserDomain USystemContext::GetUserDomain(int InUserID)
{
	for (FUser User : Computer.Users)
	{
		if (User.ID == InUserID)
		{
			return User.Domain;
		}
	}

	return EUserDomain::User;
}

FText USystemContext::GetUsername(int InUserID)
{
	for (FUser User : Computer.Users)
	{
		if (User.ID == InUserID)
		{
			return User.Username;
		}
	}
	return FText();
}

FString USystemContext::GetUserHomeDirectory(int UserID)
{
	for (FUser User : Computer.Users)
	{
		if (User.ID == UserID)
		{
			if (User.Domain == EUserDomain::Administrator)
				return TEXT("/root");
			return TEXT("/home/") + User.Username.ToString();
		}
	}

	return FString();
}

bool USystemContext::Authenticate(const FString & Username, const FString & Password, int & UserID)
{
	for (FUser User : Computer.Users)
	{
		if (User.Username.ToString() == Username && User.Password.ToString() == Password)
		{
			UserID = User.ID;
			return true;
		}
	}

	return false;
}

bool USystemContext::GetSuitableProgramForFileExtension(const FString & InExtension, UPeacegateProgramAsset *& OutProgram)
{

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
		Peacenet->UpdateComputer(Computer.ID, Computer);
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
