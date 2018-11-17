// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "UMissionUnlock.h"
#include "UUnlockPack.h"
#include "UMissionAction.h"
#include "UUnlockAction.generated.h"

/**
 * An action that unlocks the specified mission unlock when the action is executed.
 */
UCLASS(EditInlineNew, BlueprintType)
class PROJECTOGLOWIA_API UUnlockAction : public UMissionAction
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FUnlockPackEntry Unlock;

	virtual void NativeExecuteMissionAction() override;
};
