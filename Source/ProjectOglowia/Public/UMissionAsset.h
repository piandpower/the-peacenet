// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UMissionAsset.generated.h"

/**
 * Represents a mission within a Peacenet campaign.
 */
UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UMissionAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mission|Internal")
	FName InternalName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mission|User Interface")
	FText MissionTitle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mission|User Interface")
	FText Description;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mission|User Interface")
	FText Giver;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mission|User Interface")
	bool IsSideMission = false;
};
