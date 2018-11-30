// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "UPeacegateProgramAsset.h"
#include "Engine/DataAsset.h"
#include "UEmailAttachmentAsset.generated.h"

/**
 * A base class for all email attachment assets.
 */
UCLASS(BlueprintType, Abstract)
class PROJECTOGLOWIA_API UEmailAttachmentAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Email", meta = (DefaultToSelf = "OpeningProgram"))
		virtual void OpenAttachment(UProgram* OpeningProgram);
};
