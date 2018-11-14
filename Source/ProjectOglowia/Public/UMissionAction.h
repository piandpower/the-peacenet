// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "USystemContext.h"
#include "Text.h"
#include "UMissionAction.generated.h"

/**
 * Represents an action performed by a mission.
 */
UCLASS(Blueprintable, EditInlineNew, Abstract)
class PROJECTOGLOWIA_API UMissionAction : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Mission Action")
	USystemContext* SystemContext;

	UFUNCTION(BlueprintImplementableEvent, Category = "Mission Action")
	void ExecuteMissionAction();

};

UCLASS(Blueprintable, EditInlineNew, Abstract)
class PROJECTOGLOWIA_API ULatentMissionAction : public UMissionAction
{
	GENERATED_BODY()

private:
	bool bIsActionFinished = false;
	bool bIsActionFailed = false;
	
	UPROPERTY()
	FString FailReason;

public:
	UFUNCTION()
	bool IsCompleted();

	UFUNCTION()
	bool IsFailed();

	UFUNCTION()
	FString GetFailReasonText();

protected:
	UFUNCTION(BlueprintCallable, Category = "Latent Mission Action")
	void Complete();

	UFUNCTION(BlueprintCallable, Category = "Latent Mission Action")
	void Fail(const FString& InFailReason);
};