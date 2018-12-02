// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UPeacenetGameInstance.h"
#include "Kismet/GameplayStatics.h"

UPeacenetSettings * UPeacenetGameInstance::GetSettings()
{
	return this->Settings;
}

void UPeacenetGameInstance::SaveSettings()
{
	// Save the settings save to disk.
	UGameplayStatics::SaveGameToSlot(this->Settings, "PeacenetSettings", 0);

	this->SettingsApplied.Broadcast(this->Settings);
}

void UPeacenetGameInstance::LoadSettings()
{
	// Load it in.
	this->Settings = Cast<UPeacenetSettings>(UGameplayStatics::LoadGameFromSlot("PeacenetSettings", 0));
}

void UPeacenetGameInstance::Init()
{
	// Do we have a settings save?
	if (UGameplayStatics::DoesSaveGameExist("PeacenetSettings", 0))
	{
		this->LoadSettings();
	}
	else
	{
		// Create a new save.
		this->Settings = NewObject<UPeacenetSettings>();

		this->SaveSettings();
	}
}

void UPeacenetGameInstance::Shutdown()
{
	// Unreal Engine's about to shut down.	

	this->SaveSettings();
}