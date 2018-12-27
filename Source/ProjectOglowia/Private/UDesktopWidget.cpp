// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UDesktopWidget.h"
#include "PeacenetWorldStateActor.h"
#include "USystemContext.h"
#include "FComputer.h"
#include "UUserContext.h"
#include "ImageLoader.h"
#include "UWorkspace.h"
#include "PTerminalWidget.h"
#include "FPeacenetIdentity.h"
#include "CommonUtils.h"
#include "WallpaperAsset.h"
#include "UProgram.h"
#include "UConsoleContext.h"

void UDesktopWidget::ShowProgramOnWorkspace(UProgram* InProgram)
{
	UWorkspace* Workspace = this->GetCurrentWorkspace();
	if (Workspace)
		Workspace->ShowProgramOnWorkspace(InProgram);
}

void UDesktopWidget::CloseActiveProgram()
{
	this->EventActiveProgramClose.Broadcast();
}

void UDesktopWidget::ResetEventLog()
{
	this->OnClearEventLog();

	auto RootFS = this->SystemContext->GetFilesystem(0);

	if (RootFS->FileExists("/var/log/peacegate.log"))
	{
		FString LogText;
		EFilesystemStatusCode Anus;
		
		check(RootFS->ReadText("/var/log/peacegate.log", LogText, Anus));

		TArray<FEventLogEntry> LogEntries = UCommonUtils::ReadEventLogFile(LogText);

		for (auto Log : LogEntries)
		{
			this->OnAddEventToLog(Log);
		}
	}
}

USystemContext* UDesktopWidget::GetSystemContext()
{
	return this->SystemContext;
}

UProgram * UDesktopWidget::SpawnProgramFromClass(TSubclassOf<UProgram> InClass, const FText& InTitle, UTexture2D* InIcon)
{
	UWindow* OutputWindow = nullptr;

	UProgram* Program = UProgram::CreateProgram(this->SystemContext->GetPeacenet()->WindowClass, InClass, this->SystemContext, this->UserID, OutputWindow);

	OutputWindow->WindowTitle = InTitle;
	OutputWindow->Icon = InIcon;
	OutputWindow->EnableMinimizeAndMaximize = false;

	return Program;
}

void UDesktopWidget::NativeConstruct()
{
	// Reset the app launcher.
	this->ResetAppLauncher();

	// reset event log.
	this->ResetEventLog();

	// Grab the user's home directory.
	this->UserHomeDirectory = this->SystemContext->GetUserHomeDirectory(this->UserID);

	// Get the filesystem context for this user.
	this->Filesystem = this->SystemContext->GetFilesystem(this->UserID);

	// Make sure that we intercept filesystem write operations that pertain to us.
	TScriptDelegate<> FSOperation;
	FSOperation.BindUFunction(this, "OnFilesystemOperation");

	this->Filesystem->FilesystemOperation.Add(FSOperation);
	this->SystemContext->GetFilesystem(0)->FilesystemOperation.Add(FSOperation);

	// Create an image loader to use for wallpaper loading.
	this->ImageLoader = NewObject<UImageLoader>(this);

	// Create a function binding.
	TScriptDelegate<> ImageLoaded;
	ImageLoaded.BindUFunction(this, "SetWallpaper");
	this->ImageLoader->OnLoadCompleted().Add(ImageLoaded);

	// Does this user have an existing wallpaper?
	if (this->Filesystem->FileExists(this->UserHomeDirectory + TEXT("/.peacegate/wallpaper.png")))
	{
		this->ImageLoader->LoadImageAsync(this->Filesystem, this, this->UserHomeDirectory + TEXT("/.peacegate/wallpaper.png"));
	}
	else 
	{
		// We need to find and set the default wallpaper.
		for (auto WallpaperAsset : this->SystemContext->GetPeacenet()->Wallpapers)
		{
			if (WallpaperAsset->IsDefault)
			{
				this->SetWallpaper(WallpaperAsset->WallpaperTexture);
				this->Filesystem->WriteBinary(this->UserHomeDirectory + TEXT("/.peacegate/wallpaper.png"), UImageLoader::GetBitmapData(this->WallpaperTexture));
				break;
			}
		}
	}

	Super::NativeConstruct();
}

UConsoleContext * UDesktopWidget::CreateConsole(UPTerminalWidget* InTerminal)
{
	// DEPRECATED IN FAVOUR OF UUserContext::CreateConsole().
	return this->SystemContext->GetUserContext(this->UserID)->CreateConsole(InTerminal);
}

void UDesktopWidget::SetWallpaper(UTexture2D* InTexture)
{
	this->WallpaperTexture = InTexture;
}

void UDesktopWidget::OnFilesystemOperation(EFilesystemEventType InType, FString InPath)
{
	switch (InType)
	{
		case EFilesystemEventType::WriteFile:
			if (InPath == this->UserHomeDirectory + TEXT("/.peacegate/wallpaper.png"))
			{
				// Wallpaper update.
				this->WallpaperTexture = UImageLoader::LoadImageFromDisk(this->Filesystem, this, this->UserHomeDirectory + TEXT("/.peacegate/wallpaper.png"));

				this->EnqueueNotification(FText::FromString("New wallpaper"), FText::FromString("A new wallpaper has been set by a program."), this->WallpaperTexture);

				this->SystemContext->GetPeacenet()->SaveWorld();
			}
			else if (InPath == "/var/log/peacegate.log")
			{
				this->ResetEventLog();
			}
			break;
	}
}

void UDesktopWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	check(this->SystemContext);
	check(this->SystemContext->GetPeacenet());

	this->MyCharacter = this->SystemContext->GetCharacter();
	this->MyComputer = this->SystemContext->GetComputer();

	// If a notification isn't currently active...
	if (!bIsWaitingForNotification)
	{
		// Do we have notifications left in the queue?
		if (this->NotificationQueue.Num())
		{
			// Grab the first notification.
			FDesktopNotification Note = this->NotificationQueue[0];

			// Remove it from the queue - we have a copy.
			this->NotificationQueue.RemoveAt(0);

			// We're waiting again.
			bIsWaitingForNotification = true;

			// Set the notification values.
			this->NotificationTitle = Note.Title;
			this->NotificationMessage = Note.Message;
			this->NotificationIcon = Note.Icon;

			// Fire the event.
			this->OnShowNotification();
		}
	}

	// update username.
	this->CurrentUsername = this->SystemContext->GetUsername(this->UserID);

	// update hostname
	this->CurrentHostname = this->SystemContext->GetHostname();

	// And now the Peacenet name.
	this->CurrentPeacenetName = this->MyCharacter.CharacterName;

	this->TimeOfDay = this->SystemContext->GetPeacenet()->GetTimeOfDay();

	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UDesktopWidget::ResetAppLauncher()
{
	// Clear the main menu.
	this->ClearAppLauncherMainMenu();

	// Collect app launcher categories.
	TArray<FString> CategoryNames;
	for (auto ProgramName : this->SystemContext->GetComputer().InstalledPrograms)
	{
		UPeacegateProgramAsset* Program;

		if (!this->SystemContext->GetPeacenet()->FindProgramByName(ProgramName, Program))
			continue;
		if (!CategoryNames.Contains(Program->AppLauncherItem.Category.ToString()))
		{
			CategoryNames.Add(Program->AppLauncherItem.Category.ToString());
			this->AddAppLauncherMainMenuItem(Program->AppLauncherItem.Category);
		}
	}
}

void UDesktopWidget::EnqueueNotification(const FText & InTitle, const FText & InMessage, UTexture2D * InIcon)
{
	FDesktopNotification note;
	note.Title = InTitle;
	note.Message = InMessage;
	note.Icon = InIcon;
	this->NotificationQueue.Add(note);
}

void UDesktopWidget::ResetWindowList()
{
}

void UDesktopWidget::ResetDesktopIcons()
{
}

void UDesktopWidget::ShowAppLauncherCategory(const FString& InCategoryName)
{
	// Clear the sub-menu.
	this->ClearAppLauncherSubMenu();

	// Add all the programs.
	for (auto ProgramId : this->SystemContext->GetComputer().InstalledPrograms)
	{
		UPeacegateProgramAsset* Program = nullptr;

		if (!this->SystemContext->GetPeacenet()->FindProgramByName(ProgramId, Program))
			continue;

		if (Program->AppLauncherItem.Category.ToString() != InCategoryName)
			continue;

		// because Blueprints hate strings being passed by-value.
		FString ProgramName = ProgramId.ToString();

		// Add the item. Woohoo.
		this->AddAppLauncherSubMenuItem(Program->AppLauncherItem.Name, Program->AppLauncherItem.Description, ProgramName, Program->AppLauncherItem.Icon);
	}
}

bool UDesktopWidget::OpenProgram(const FName InExecutableName, UProgram*& OutProgram)
{
	return this->SystemContext->OpenProgram(InExecutableName, OutProgram);
}

void UDesktopWidget::FinishShowingNotification()
{
	bIsWaitingForNotification = false;
}