// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UPeacenetGameInstance.h"
#include "UGameTypeAsset.h"
#include "AssetRegistry/Public/IAssetRegistry.h"
#include "AssetRegistry/Public/AssetRegistryModule.h"
#include "Kismet/GameplayStatics.h"

void UPeacenetGameInstance::CreateWorld(FString InCharacterName, UPeacenetGameTypeAsset* InGameType)
{

}

TArray<UPeacenetGameTypeAsset*> const& UPeacenetGameInstance::GetGameTypes() const
{
	return this->GameTypes;
}

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

		// Get the Asset Registry
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

	// A place to store computer type asset data
	TArray<FAssetData> Assets;

	if (!AssetRegistryModule.Get().GetAssetsByClass("PeacenetGameTypeAsset", Assets, true))
		check(false);

	for (auto& Asset : Assets)
	{
		this->GameTypes.Add(Cast<UPeacenetGameTypeAsset>(Asset.GetAsset()));
	}
}

void UPeacenetGameInstance::Shutdown()
{
	// Unreal Engine's about to shut down.	

	this->SaveSettings();
}