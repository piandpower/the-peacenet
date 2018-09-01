// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectiveHintWidget.h"
#include "UMissionAsset.generated.h"

USTRUCT(BlueprintType)
struct FMissionInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Giver;
};

UCLASS(EditInlineNew, Blueprintable)
class UMissionObjective : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="Mission")
	void Start(const APlayerController* InPlayerController);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Mission")
	void End();

	UFUNCTION(BlueprintImplementableEvent, Category = "Mission")
	void OnStateReset();

	UFUNCTION(BlueprintCallable, Category = "Mission")
	void ResetObjectiveState()
	{
		failReason = TEXT("");
		isFailed = false;
		isComplete = false;

		OnStateReset();
	}

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Mission")
	void Tick(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category="Mission")
	bool IsComplete() { return isComplete; }

	UFUNCTION(BlueprintCallable, Category = "Mission")
		bool IsFailed(FString& OutFailReason)
	{
		OutFailReason = failReason;
		return isFailed;
	}

private:
	FString failReason;
	bool isFailed = false;
	bool isComplete = false;


protected:
	UFUNCTION(BlueprintCallable, Category = "Mission")
	void CompleteObjective() 
	{
		isComplete = true;
	}

	UFUNCTION(BlueprintCallable, Category="Mission")
	void FailObjective(const FString& InFailReason)
	{
		isFailed = true;
		failReason = InFailReason;
	}
};

UCLASS(Blueprintable, EditInlineNew)
class UObjectiveEvent : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void Trigger(const APlayerController* Player);
};

USTRUCT(BlueprintType)
struct FObjectiveInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText Description;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced)
	UMissionObjective* ObjectiveClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced)
	UObjectiveHintWidget* ObjectiveHintWidget;

	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced)
	UObjectiveEvent* OnStartEvent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced)
	UObjectiveEvent* OnCompletedEvent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced)
	UObjectiveEvent* OnFailedEvent;
};


/**
 * Represents a Peacenet mission asset.
 */
UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UMissionAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMissionInfo MissionInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<UMissionAsset>> RequiredMissions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FObjectiveInfo> Objectives;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAbandonable = true;
};
