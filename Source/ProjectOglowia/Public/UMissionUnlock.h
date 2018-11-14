// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "Text.h"
#include "UPeacenetSaveGame.h"
#include "USystemContext.h"
#include "UMissionUnlock.generated.h"

/**
 * An object representing something the player can unlock in a mission.
 */
UCLASS(Abstract, BlueprintType, EditInlineNew)
class PROJECTOGLOWIA_API UMissionUnlock : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
		virtual void PerformUnlock(USystemContext* InSystemContext, UPeacenetSaveGame* InSaveGame) {}

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText Description;

};
