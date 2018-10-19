// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UPeacegateProgramAsset.h"
#include "CommonUtils.h"
#include "PTerminalWidget.h"
#include "UConsoleContext.h"
#include "UPeacegateFileSystem.h"

UProgram* UProgram::CreateProgram(const TSubclassOf<UWindow> InWindow, const TSubclassOf<UProgram> InProgramClass, const TScriptInterface<ISystemContext> InSystem, const int InUserID, UWindow*& OutWindow)
{
	// The window is what contains the program's UI.
	UWindow* Window = NewObject<UWindow>(InSystem.GetObject(), InWindow);

	// Construct the actual program.
	UProgram* ProgramInstance = NewObject<UProgram>(Window, InProgramClass);

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

FText UProgram::GetUsername()
{
	FUserInfo User = ISystemContext::Execute_GetUserInfo(this->Window->SystemContext.GetObject(), this->Window->UserID);
	return FText::FromString(User.Username);
}

FText UProgram::GetHostname()
{
	return FText::FromString(ISystemContext::Execute_GetHostname(this->Window->SystemContext.GetObject()));
}

UConsoleContext* UProgram::CreateConsole(UPTerminalWidget* InTerminalWidget)
{
	UConsoleContext* Console = NewObject<UConsoleContext>(this);

	// Assign it to the terminal widget.
	Console->Terminal = InTerminalWidget;

	// User ID matches our window.
	Console->UserID = Window->UserID;

	// Same with system ctx.
	Console->SystemContext = Window->SystemContext;

	// Get user info.
	FUserInfo User = ISystemContext::Execute_GetUserInfo(Console->SystemContext.GetObject(), Console->UserID);

	// If the user's username is root, then we set the home directory to "/root."
	if (User.IsAdminUser)
	{
		Console->HomeDirectory = TEXT("/root");
	}
	else
	{
		Console->HomeDirectory = TEXT("/home/") + User.Username;
	}

	Console->WorkingDirectory = Console->HomeDirectory;
	
	// Console FS matches ours.
	Console->Filesystem = this->Filesystem;

	// Attempt to create the user directory if it's not there
	if (!Console->Filesystem->DirectoryExists(Console->WorkingDirectory))
	{
		EFilesystemStatusCode StatusCode;

		if (!Console->Filesystem->CreateDirectory(Console->WorkingDirectory, StatusCode))
		{
			FText Error = UCommonUtils::GetFriendlyFilesystemStatusCode(StatusCode);
			Console->Write(TEXT("peacegate: user home directory '`8") + Console->WorkingDirectory + TEXT("`1' could not be created: "));
			Console->WriteLine(Error.ToString());

			// Working directory becomes /.
			Console->WorkingDirectory = TEXT("/");
		}
	}

	return Console;
}


void UProgram::ShowInfo(const FText & InTitle, const FText & InMessage, const EInfoboxIcon InIcon)
{
	Window->ShowInfo(InTitle, InMessage, InIcon);
}

FString UProgram::HomeDirectory()
{
	return ISystemContext::Execute_GetUserHomeDirectory(Window->SystemContext.GetObject(), Window->UserID);
}

void UProgram::AskForFile(const FString InBaseDirectory, const FString InFilter, const EFileDialogType InDialogType, const FFileDialogDismissedEvent & OnDismissed)
{
	Window->AskForFile(InBaseDirectory, InFilter, InDialogType, OnDismissed);
}

void UProgram::SetupContexts()
{
	// Fetch a filesystem context from Peacegate, with the current User ID.
	this->Filesystem = ISystemContext::Execute_GetFilesystem(Window->SystemContext.GetObject(), Window->UserID);

	// Show the program on the current workspace.
	ISystemContext::Execute_ShowWindowOnWorkspace(Window->SystemContext.GetObject(), this);

	// Add ourself to the window's client slot.
	Window->AddWindowToClientSlot(this);
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
	if (!ISystemContext::Execute_GetSuitableProgramForFileExtension(Window->SystemContext.GetObject(), Extension, ProgramAsset))
	{
		OutStatus = EProgramFileOpenStatus::NoSuitableProgram;
		return false;
	}

	TSubclassOf<UWindow> WindowClass(Window->GetClass());

	UWindow* NewWindow;
	UProgram* NewProgram = UProgram::CreateProgram(WindowClass, ProgramAsset->ProgramClass, Window->SystemContext, Window->UserID, NewWindow);

	NewWindow->WindowTitle = FText::FromString(ProgramAsset->AppLauncherItem.Name);
	NewWindow->Icon = ProgramAsset->AppLauncherItem.Icon;
	NewWindow->EnableMinimizeAndMaximize = ProgramAsset->AppLauncherItem.EnableMinimizeAndMaximize;

	NewProgram->FileOpened(InPath);

	return true;
}
