// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "UMissionUnlock.h"
#include "UUnlockPack.generated.h"

USTRUCT(BlueprintType)
struct PROJECTOGLOWIA_API FUnlockPackEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Instanced)
	UMissionUnlock* Unlock;
};

/**
 * Represents a group of unlocks that are unlocked all at once and treated as a single unlock.
 */
UCLASS(BlueprintType, EditInlineNew)
class PROJECTOGLOWIA_API UUnlockPack : public UMissionUnlock
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Editanywhere)
	TArray<FUnlockPackEntry> ChildUnlocks;

	virtual void PerformUnlock(USystemContext* InSystemContext, UPeacenetSaveGame* InSaveGame) override;
};
