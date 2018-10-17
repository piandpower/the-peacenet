// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/Texture2D.h"
#include "UHackableAsset.h"

#include "UComputerTypeAsset.generated.h"

/**
 * Represents a type of computer that we can generate in a Peacenet world.
 */
UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UComputerTypeAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName InternalID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText DisplayName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UHackableAsset*> Services;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Rarity;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTexture2D* Icon;
};
