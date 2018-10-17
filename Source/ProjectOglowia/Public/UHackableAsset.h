// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/Texture2D.h"

#include "UHackableAsset.generated.h"

/**
 * Represents a hackable service within The Peacenet.
 */
UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UHackableAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName InternalID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText DisplayName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Port;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTexture2D* Icon;
};
