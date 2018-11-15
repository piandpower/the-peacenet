// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UPeacegateProgramAsset.h"
#include "CommonUtils.h"
#include "PTerminalWidget.h"
#include "UMissionAsset.h"
#include "UConsoleContext.h"
#include "UPeacegateFileSystem.h"
#include "PeacenetWorldStateActor.h"
#include "Kismet/GameplayStatics.h"

void UProgram::PushNotification(const FText & InNotificationMessage)
{
	this->Window->SystemContext->Desktop->EnqueueNotification(this->Window->WindowTitle, InNotificationMessage, this->Window->Icon);
}

UProgram* UProgram::CreateProgram(const TSubclassOf<UWindow> InWindow, const TSubclassOf<UProgram> InProgramClass, USystemContext* InSystem, const int InUserID, UWindow*& OutWindow)
{
	check(InSystem->Peacenet);

	APlayerController* MyPlayer = UGameplayStatics::GetPlayerController(InSystem->Peacenet->GetWorld(), 0);

	// The window is what contains the program's UI.
	UWindow* Window = CreateWidget<UWindow, APlayerController>(MyPlayer, InWindow);

	// Construct the actual program.
	UProgram* ProgramInstance = CreateWidget<UProgram, APlayerController>(MyPlayer, InProgramClass);

	// Program and window are friends with each other
	ProgramInstance->Window = Window;

	// Window gets the system context and user ID.
	Window->SystemContext = InSystem;
	Window->UserID = InUserID;

	// Sets up things like the console context, FS context, etc for the current user.
	// This also injects the program UI into the window.
	ProgramInstance->SetupContexts();

	// Return the window and program.
	OutWindow = Window;
	return ProgramInstance;
}

void UProgram::ShowInfoWithCallbacks(const FText & InTitle, const FText & InMessage, const EInfoboxIcon InIcon, const EInfoboxButtonLayout ButtonLayout, const bool ShowTextInput, const FInfoboxDismissedEvent & OnDismissed, const FInfoboxInputValidator & ValidatorFunction)
{
	Window->ShowInfoWithCallbacks(InTitle, InMessage, InIcon, ButtonLayout, ShowTextInput, OnDismissed, ValidatorFunction);
}

void UProgram::StartMission(UMissionAsset* InMissionAsset)
{
	if (!this->IsMissionActive() && this->MissionsEnabled())
	{
		if (this->Window->SystemContext->Peacenet->StartMission(InMissionAsset, this->Window->SystemContext))
		{
			this->Window->Close();
		}
	}
}

bool UProgram::GetAvailableMissions(TArray<UMissionAsset*>& OutMissions)
{
	if (this->IsMissionActive() || !this->MissionsEnabled())
	{
		return false;
	}

	for (auto Mission : this->Window->SystemContext->Peacenet->Missions)
	{
		if (this->Window->SystemContext->Peacenet->SaveGame->Missions.Contains(Mission->InternalID))
			continue;

		bool shouldAdd = true;

		for (auto SubMission : Mission->Prerequisites)
		{
			if (!this->Window->SystemContext->Peacenet->SaveGame->Missions.Contains(SubMission->InternalID))
			{
				shouldAdd = false;
				break;
			}
		}

		if (shouldAdd)
		{
			OutMissions.Add(Mission);
		}
	}
	
	return OutMissions.Num() > 0;
}

bool UProgram::IsMissionActive()
{
	return this->Window->SystemContext->Peacenet->IsMissionActive();
}

bool UProgram::MissionsEnabled()
{
	if (this->Window->SystemContext->Computer.OwnerType != EComputerOwnerType::Player)
	{
		return false;
	}
	return this->Window->SystemContext->Peacenet->GameType->EnableMissions;
}

FText UProgram::GetUsername()
{
	FUserInfo User = Window->SystemContext->GetUserInfo(Window->UserID);
	return FText::FromString(User.Username);
}

FText UProgram::GetHostname()
{
	return FText::FromString(this->Window->SystemContext->GetHostname());
}

UConsoleContext* UProgram::CreateConsole(UPTerminalWidget* InTerminalWidget)
{
	UConsoleContext* SubConsole = NewObject<UConsoleContext>(this);

	// Assign it to the terminal widget.
	SubConsole->Terminal = InTerminalWidget;

	// User ID matches our window.
	SubConsole->UserID = Window->UserID;

	// Same with system ctx.
	SubConsole->SystemContext = Window->SystemContext;

	// Get user info.
	FUserInfo User = SubConsole->SystemContext->GetUserInfo(SubConsole->UserID);

	// If the user's username is root, then we set the home directory to "/root."
	if (User.IsAdminUser)
	{
		SubConsole->HomeDirectory = TEXT("/root");
	}
	else
	{
		SubConsole->HomeDirectory = TEXT("/home/") + User.Username;
	}

	SubConsole->WorkingDirectory = SubConsole->HomeDirectory;
	
	// Console FS matches ours.
	SubConsole->Filesystem = this->Filesystem;

	// Attempt to create the user directory if it's not there
	if (!SubConsole->Filesystem->DirectoryExists(SubConsole->WorkingDirectory))
	{
		EFilesystemStatusCode StatusCode;

		if (!SubConsole->Filesystem->CreateDirectory(SubConsole->WorkingDirectory, StatusCode))
		{
			FText Error = UCommonUtils::GetFriendlyFilesystemStatusCode(StatusCode);
			SubConsole->Write(TEXT("peacegate: user home directory '`8") + Console->WorkingDirectory + TEXT("`1' could not be created: "));
			SubConsole->WriteLine(Error.ToString());

			// Working directory becomes /.
			SubConsole->WorkingDirectory = TEXT("/");
		}
	}

	return SubConsole;
}


void UProgram::ShowInfo(const FText & InTitle, const FText & InMessage, const EInfoboxIcon InIcon)
{
	Window->ShowInfo(InTitle, InMessage, InIcon);
}

FString UProgram::HomeDirectory()
{
	return Window->SystemContext->GetUserHomeDirectory(Window->UserID);
}

void UProgram::AskForFile(const FString InBaseDirectory, const FString InFilter, const EFileDialogType InDialogType, const FFileDialogDismissedEvent & OnDismissed)
{
	Window->AskForFile(InBaseDirectory, InFilter, InDialogType, OnDismissed);
}

void UProgram::SetupContexts()
{
	// Fetch a filesystem context from Peacegate, with the current User ID.
	this->Filesystem = Window->SystemContext->GetFilesystem(Window->UserID);

	// Show the program on the current workspace.
	Window->SystemContext->ShowWindowOnWorkspace(this);

	// Add ourself to the window's client slot.
	Window->AddWindowToClientSlot(this);

	this->NativeProgramLaunched();
}

void UProgram::SetWindowMinimumSize(FVector2D InSize)
{
	Window->SetClientMinimumSize(InSize);
}

bool UProgram::OpenFile(const FString & InPath, EProgramFileOpenStatus & OutStatus)
{
	if (!Filesystem)
	{
		OutStatus = EProgramFileOpenStatus::PermissionDenied;
		return false;
	}

	if (!Filesystem->FileExists(InPath))
	{
		OutStatus = EProgramFileOpenStatus::FileNotFound;
		return false;
	}

	FString Path;
	FString Extension;
	if (!InPath.Split(TEXT("."), &Path, &Extension, ESearchCase::IgnoreCase, ESearchDir::FromEnd))
	{
		OutStatus = EProgramFileOpenStatus::NoSuitableProgram;
		return false;
	}

	UPeacegateProgramAsset* ProgramAsset;
	if (!Window->SystemContext->GetSuitableProgramForFileExtension(Extension, ProgramAsset))
	{
		OutStatus = EProgramFileOpenStatus::NoSuitableProgram;
		return false;
	}

	TSubclassOf<UWindow> WindowClass(Window->GetClass());

	UWindow* NewWindow;
	UProgram* NewProgram = UProgram::CreateProgram(WindowClass, ProgramAsset->ProgramClass, Window->SystemContext, Window->UserID, NewWindow);

	NewWindow->WindowTitle = ProgramAsset->AppLauncherItem.Name;
	NewWindow->Icon = ProgramAsset->AppLauncherItem.Icon;
	NewWindow->EnableMinimizeAndMaximize = ProgramAsset->AppLauncherItem.EnableMinimizeAndMaximize;

	NewProgram->FileOpened(InPath);

	return true;
}

void UProgram::NativeProgramLaunched() {}

void USettingsProgram::NativeProgramLaunched()
{
	this->Desktop = this->Window->SystemContext->Desktop;
}