// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UPeacegateFileSystem.h"
#include "UFileAsset.generated.h"

/**
 * A data asset that tells Peacenet how to generate files for a Peacegate service.
 */
UCLASS(Blueprintable, Abstract)
class PROJECTOGLOWIA_API UFileAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual FString GetBase64();
};
