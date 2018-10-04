// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "USeasonalItemAsset.generated.h"

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	Common,
	Uncommon,
	Rare,
	Epic,
	Legendary,
	Mythic
};

/**
 * Represents an item that can be unlocked as a seasonal reward within Multiplayer.
 */
UCLASS(Blueprintable)
class PROJECTOGLOWIA_API USeasonalItemAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Metadata")
	FName ItemID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Metadata")
	EItemRarity ItemRarity = EItemRarity::Common;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "User Interface")
	FText Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "User Interface")
	FText Description;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "User Interface")
	UTexture2D* Thumbnail;
};
