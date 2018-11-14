// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "Text.h"
#include "Engine/DataAsset.h"
#include "UMissionAsset.generated.h"

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
};
