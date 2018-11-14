// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "UMissionAction.h"
#include "USetSaveBoolean.generated.h"


/**
 * A mission action that sets a save boolean to the specified value.
 */
UCLASS(BlueprintType)
class PROJECTOGLOWIA_API USetSaveBoolean : public UMissionAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName BooleanName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool NewValue = true;

protected:
	virtual void NativeExecuteMissionAction() override;
};
