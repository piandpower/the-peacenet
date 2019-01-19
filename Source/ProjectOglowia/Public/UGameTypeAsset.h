// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "FGameRules.h"
#include "Engine/Texture2D.h"
#include "Engine/DataAsset.h"
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
	bool AllowCustomSpawnCountry = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool AllowCustomGameRules = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameRules GameRules;
};