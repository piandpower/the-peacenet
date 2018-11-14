// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "Text.h"
#include "Engine/DataAsset.h"
#include "UMissionAction.h"
#include "UMissionAsset.generated.h"

/** 
 * Contains information about a mission action to start.
 */
USTRUCT(BlueprintType)
struct PROJECTOGLOWIA_API FMissionActionInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced)
	UMissionAction* Action;
};

/**
 * Represents a playable mission.
 */
UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UMissionAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FName InternalID;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText MissionName;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText Description;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<UMissionAsset*> Prerequisites;

	UPROPERTY(BlueprintReadOnly, Editanywhere)
	TArray<FMissionActionInfo> Actions;
};
