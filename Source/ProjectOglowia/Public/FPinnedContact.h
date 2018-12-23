// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "FContactNote.h"
#include "FPinnedContact.generated.h"

UENUM(BlueprintType)
enum class EPinnedContactType : uint8
{
	Person,
	Business
};

/**
 * A structure representing a pinned contact in the game.
 */
USTRUCT(BlueprintType)
struct PROJECTOGLOWIA_API FPinnedContact
{
	GENERATED_BODY()

public:
	UPROPERTY()
	EPinnedContactType ContactType = EPinnedContactType::Person;

	UPROPERTY()
	int EntityID = 0;

	UPROPERTY()
	int OwningEntityID = 0;

	UPROPERTY()
	bool IsStoryIntegral = false;


	// DISCOVERY SYSTEM VARIABLES
	UPROPERTY()
	bool IsEmailKnown = false;

	UPROPERTY()
	bool IsNameKnown = false;

	UPROPERTY()
	bool IsCountryKnown = false;
	
	UPROPERTY()
	bool IsPersonalIPKnown = false;
	
	UPROPERTY()
	bool IsPersonalHostnameKnown = false;
	
	UPROPERTY()
	bool IsPersonalUsernameKnown = false;
	
	UPROPERTY()
	TArray<int> KnownFriendIDs;
	
	UPROPERTY()
	TArray<int> KnownEnemyIDs;
	
	UPROPERTY()
	bool IsGroupMembershipKnown = false;

	UPROPERTY()
	bool IsReputationKnown = false;

	UPROPERTY()
	TArray<FContactNote> UserNotes;
};