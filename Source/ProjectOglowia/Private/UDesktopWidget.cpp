// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UDesktopWidget.h"
#include "PeacenetWorldStateActor.h"
#include "USystemContext.h"
#include "FComputer.h"
#include "UPeacegateProgramAsset.h"

void UDesktopWidget::NativeConstruct()
{
	this->ResetAppLauncher();

	Super::NativeConstruct();
}

void UDesktopWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	check(this->SystemContext);
	check(this->SystemContext->Peacenet);

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
	this->CurrentHostname = FText::FromString(this->SystemContext->GetHostname());

	// And now the Peacenet name.
	this->CurrentPeacenetName = this->SystemContext->Character.CharacterName;

	this->TimeOfDay = this->SystemContext->Peacenet->GetTimeOfDay();

	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UDesktopWidget::ResetAppLauncher()
{
	// Clear the main menu.
	this->ClearAppLauncherMainMenu();

	// Collect app launcher categories.
	TArray<FString> CategoryNames;
	for (auto ProgramName : SystemContext->Computer.InstalledPrograms)
	{
		UPeacegateProgramAsset* Program;

		if (!this->SystemContext->Peacenet->FindProgramByName(ProgramName, Program))
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
	for (auto ProgramId : this->SystemContext->Computer.InstalledPrograms)
	{
		UPeacegateProgramAsset* Program = nullptr;

		if (!this->SystemContext->Peacenet->FindProgramByName(ProgramId, Program))
			continue;

		if (Program->AppLauncherItem.Category.ToString() != InCategoryName)
			continue;

		// because Blueprints hate strings being passed by-value.
		FString ProgramName = ProgramId.ToString();

		// Add the item. Woohoo.
		this->AddAppLauncherSubMenuItem(Program->AppLauncherItem.Name, Program->AppLauncherItem.Description, ProgramName, Program->AppLauncherItem.Icon);
	}
}

void UDesktopWidget::OpenProgram(const FName InExecutableName)
{
	this->SystemContext->OpenProgram(InExecutableName);
}

void UDesktopWidget::FinishShowingNotification()
{
	bIsWaitingForNotification = false;
}