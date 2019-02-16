/********************************************************************************
 * The Peacenet - bit::phoenix("software");
 * 
 * MIT License
 *
 * Copyright (c) 2018-2019 Michael VanOverbeek, Declan Hoare, Ian Clary, 
 * Trey Smith, Richard Moch, Victor Tran and Warren Harris
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * Contributors:
 *  - Michael VanOverbeek <alkaline@bitphoenixsoftware.com>
 *
 ********************************************************************************/


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "FPeacenetIdentity.h"
#include "FCharacterRelationship.h"
#include "UDesktopWidget.h"
#include "FAdjacentNode.h"
#include "FComputer.h"
#include "UWindow.h"
#include "FEntityPosition.h"
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
	TArray<FEntityPosition> EntityPositions;

	UPROPERTY(VisibleAnywhere, Category = "Entities")
	TArray<FPeacenetIdentity> Characters;

	UPROPERTY(VisibleAnywhere, Category = "World")
	float EpochTime = 43200.f;

	UPROPERTY(VisibleAnywhere, Category = "Unlocks and Game State")
	TMap<FString, bool> Booleans;

	UPROPERTY(VisibleAnywhere, Category = "Networking")
	TMap<FString, FString> DomainNameMap;

	UPROPERTY(VisibleAnywhere, Category = "Missions")
	TArray<FName> Missions;

	UPROPERTY(VisibleAnywhere, Category = "Procgen")
	TMap<ECountry, uint8> CountryIPRanges;

	UPROPERTY(VisibleAnywhere, Category = "Procgen")
	TMap<FString, int> ComputerIPMap;
	
	UPROPERTY(VisibleAnywhere, Category = "Master Password Table")
	TArray<FAdjacentNode> AdjacentNodes;

	UPROPERTY(VisibleAnywhere, Category = "Master Password Table")
	int WorldSeed = -1;

	UPROPERTY(VisibleAnywhere, Category = "Adjacent nodes")
	TArray<int> PlayerDiscoveredNodes;

	const float SECONDS_DAY_LENGTH = 86400.f;

	UFUNCTION()
	bool IsTrue(FString InKey);

	UFUNCTION()
	void SetValue(FString InKey, bool InValue);

	UFUNCTION()
	void FixEntityIDs();

	UFUNCTION()
	TArray<int> GetAdjacents(int Node);

	UFUNCTION()
	TArray<int> GetAllEntitiesInCountry(ECountry InCountry);

	UFUNCTION()
	void AddAdjacent(int NodeA, int NodeB);

	UFUNCTION()
	void RemoveAdjacent(int NodeA, int NodeB);

	UFUNCTION()
	bool AreAdjacent(int NodeA, int NodeB);

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

	UFUNCTION()
	void SetEntityPosition(int EntityID, FVector2D Position);

	UFUNCTION()
	bool GetPosition(int EntityID, FVector2D& OutPosition);

	UFUNCTION()
	int GetSkillOf(FComputer& InComputer);

	UFUNCTION()
	bool LocationTooCloseToEntity(ECountry InCountry, FVector2D InLocation, float InMinimumDistance);
};