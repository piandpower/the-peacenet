// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "UMissionAction.h"
#include "UCheckSaveBoolean.generated.h"

/**
 * A mission action that checks a save boolean.
 */
UCLASS(BlueprintType)
class PROJECTOGLOWIA_API UCheckSaveBoolean : public ULatentMissionAction
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName BooleanName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool ExpectedResult = true;

protected:
	virtual void NativeTick(float InDeltaTime) override;
};