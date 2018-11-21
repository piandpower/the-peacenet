// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "FComputer.h"
#include "FPeacenetIdentity.h"
#include "UDesktopWidget.h"
#include "FEnterpriseNetwork.h"
#include "UWindow.h"
#include "UPeacenetSaveGame.generated.h"

class UDesktopWidget;

/**
 * Represents a world state within Peacenet
 */
UCLASS()
class PROJECTOGLOWIA_API UPeacenetSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "Peacegate")
	int PlayerCharacterID = 0;

	UPROPERTY(VisibleAnywhere, Category = "Peacegate")
	int PlayerUserID = 0;

	UPROPERTY(VisibleAnywhere, Category = "Peacegate")
	TSubclassOf<UWindow> WindowClass;

	UPROPERTY(VisibleAnywhere, Category = "Peacegate")
	TSubclassOf<UDesktopWidget> DesktopClass;

	UPROPERTY(VisibleAnywhere, Category = "Peacegate")
	FString GameTypeName;

	UPROPERTY(VisibleAnywhere, Category = "Entities")
	TArray<FComputer> Computers;

	UPROPERTY(VisibleAnywhere, Category = "Entities")
	TArray<FPeacenetIdentity> Characters;

	UPROPERTY(VisibleAnywhere, Category = "World")
	float EpochTime = 43200.f;

	UPROPERTY(VisibleAnywhere, Category = "World")
	TArray<FEnterpriseNetwork> Businesses;

	UPROPERTY(VisibleAnywhere, Category = "Unlocks and Game State")
	TMap<FName, bool> Booleans;

	UPROPERTY(VisibleAnywhere, Category = "Networking")
	TMap<FString, FString> DomainNameMap;

	UPROPERTY(VisibleAnywhere, Category = "Missions")
	TArray<FName> Missions;

	UPROPERTY(VisibleAnywhere, Category = "NetMap")
	TArray<int> DiscoveredCharacterIds;

	const float SECONDS_DAY_LENGTH = 86400.f;

	UFUNCTION()
	bool CharacterNameExists(FText CharacterName);

	UFUNCTION()
	bool CompanyNameExists(FText CompanyName);

	UFUNCTION()
	bool DomainNameExists(FString InDomainName);

	UFUNCTION()
	bool IPAddressAllocated(FString InIPAddress);

	UFUNCTION()
	bool IsCharacterNodePositionTaken(ECountry InCountry, FVector2D InPosition);

	UFUNCTION()
	bool CountryHasEmailService(ECountry InCountry);
};
