// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/Texture2D.h"
#include "WallpaperAsset.generated.h"

/**
 * Represents a Peacegate OS wallpaper.
 */
UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UWallpaperAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName InternalID;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText FriendlyName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTexture2D * WallpaperTexture;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool UnlockedByDefault = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool IsDefault = false;
};
