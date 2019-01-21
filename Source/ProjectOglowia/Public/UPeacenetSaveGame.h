// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "FComputer.h"
#include "FPeacenetIdentity.h"
#include "FCharacterRelationship.h"
#include "UDesktopWidget.h"
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
	bool IsNewGame = true;

	UPROPERTY(VisibleAnywhere, Category = "Peacegate")
	int PlayerCharacterID = 0;

	UPROPERTY(VisibleAnywhere, Category = "Peacegate")
	int PlayerUserID = 0;

	UPROPERTY(VisibleAnywhere, Category = "Peacegate")
	TSubclassOf<UWindow> WindowClass;

	UPROPERTY(VisibleAnywhere, Category = "Peacegate")
	FString GameTypeName;

	UPROPERTY(VisibleAnywhere, Category = "Peacenet")
	TArray<FCharacterRelationship> CharacterRelationships;

	UPROPERTY(VisibleAnywhere, Category = "Entities")
	TArray<FComputer> Computers;

	UPROPERTY(VisibleAnywhere, Category = "Entities")
	TArray<FPeacenetIdentity> Characters;

	UPROPERTY(VisibleAnywhere, Category = "World")
	float EpochTime = 43200.f;

	UPROPERTY(VisibleAnywhere, Category = "Unlocks and Game State")
	TMap<FName, bool> Booleans;

	UPROPERTY(VisibleAnywhere, Category = "Networking")
	TMap<FString, FString> DomainNameMap;

	UPROPERTY(VisibleAnywhere, Category = "Missions")
	TArray<FName> Missions;

	UPROPERTY(VisibleAnywhere, Category = "Procgen")
	TMap<ECountry, uint8> CountryIPRanges;

	UPROPERTY(VisibleAnywhere, Category = "Procgen")
	TMap<FString, int> ComputerIPMap;
	
	UPROPERTY(VisibleAnywhere, Category = "Master Password Table")
	TArray<FString> MPT;

	UPROPERTY(VisibleAnywhere, Category = "Master Password Table")
	int WorldSeed = -1;

	const float SECONDS_DAY_LENGTH = 86400.f;

	UFUNCTION()
	void FixEntityIDs();

	UFUNCTION()
	bool RelatesWith(int InFirstEntity, int InSecondEntity);

	UFUNCTION()
	bool CharacterNameExists(FString CharacterName);

	UFUNCTION()
	bool DomainNameExists(FString InDomainName);

	UFUNCTION()
	bool IPAddressAllocated(FString InIPAddress);

	UFUNCTION()
		bool GetCharacterByID(int InEntityID, FPeacenetIdentity& OutCharacter, int& OutIndex);

	UFUNCTION()
	bool GetComputerByID(int InEntityID, FComputer& OutComputer, int& OutIndex);
};