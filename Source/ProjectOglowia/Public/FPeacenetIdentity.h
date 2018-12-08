// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "ECountry.h"
#include "FPeacenetIdentity.generated.h"

/**
 * Represents a type of Peacenet character identity.
 */
UENUM(BlueprintType)
enum class EIdentityType : uint8
{
	Player,
	NonPlayer,
	Story
};

/**
 * Represents a character's identity within The Peacenet.
 */
USTRUCT(BlueprintType)
struct PROJECTOGLOWIA_API FPeacenetIdentity
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int ID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString CharacterName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector2D NodePosition;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Skill;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int ComputerID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Reputation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ECountry Country;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EIdentityType CharacterType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<int> Friends;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<int> Enemies;
};
