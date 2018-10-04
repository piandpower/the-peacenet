// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "PacketTypeAsset.generated.h"

/**
 * Represents a type of packet, used in crafting.
 */
UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UPacketTypeAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName ItemID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float RarityPercentage = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTexture2D* Icon;
};
