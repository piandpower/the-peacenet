// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "FComputer.h"
#include "FPeacenetIdentity.h"
#include "UStoryCharacterAsset.generated.h"

USTRUCT(Blueprintable)
struct FComputerGenerationInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText Hostname;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EComputerType ComputerType;
};

UENUM(BlueprintType)
enum class EStoryCharacterPlayerRelationship : uint8
{
	None,
	Friend,
	Enemy
};

/**
 * Represents a story character.
 */
UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UStoryCharacterAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText CharacterName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText PersonalComputerHostname;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float StartingReputation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int StartingSkill;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FComputerGenerationInfo> AdditionalComputers;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EStoryCharacterPlayerRelationship RelationshipWithPlayer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UStoryCharacterAsset*> Friends;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UStoryCharacterAsset*> Enemies;
};
