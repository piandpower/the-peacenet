// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "USystemContext.h"
#include "Text.h"
#include "UPeacenetSaveGame.h"
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

	UPROPERTY(BlueprintReadOnly, Category = "Mission Action")
	UPeacenetSaveGame* SaveGame;

	UFUNCTION(BlueprintImplementableEvent, Category = "Mission Action")
	void K2_ExecuteMissionAction();

	UFUNCTION()
	virtual void NativeExecuteMissionAction() {}

	UFUNCTION()
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

	UFUNCTION()
	void Tick(float InDeltaTime);

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Latent Mission Action")
	void K2_Tick(float InDeltaTime);

	UFUNCTION()
	virtual void NativeTick(float InDeltaTime) {}

	UFUNCTION(BlueprintCallable, Category = "Latent Mission Action")
	void Complete();

	UFUNCTION(BlueprintCallable, Category = "Latent Mission Action")
	void Fail(const FString& InFailReason);
};