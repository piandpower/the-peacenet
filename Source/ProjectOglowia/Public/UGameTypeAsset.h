// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "ECountry.h"
#include "UMissionAsset.h"
#include "UGameTypeAsset.generated.h"

/**
 * An asset that describes a Peacenet local virtual machine (singleplayer game mode)
 */
UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UPeacenetGameTypeAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Description;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTexture2D* Icon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ECountry SpawnCountry;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool EnableMissions = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool UnlockAllProgramsByDefault = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UMissionAsset* TutorialMission;
};