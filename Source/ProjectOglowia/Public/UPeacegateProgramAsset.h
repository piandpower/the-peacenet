// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "UWindow.h"
#include "UPeacegateProgramAsset.generated.h"

UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UProgram : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	UWindow* Window;

	UFUNCTION(BlueprintCallable)
		void SetWindowMinimumSize(FVector2D InSize)
	{
		Window->SetClientMinimumSize(InSize);
	}

	UFUNCTION(BlueprintCallable)
	void InjectIntoWindow()
	{
		Window->AddWindowToClientSlot(this);
	}
};

USTRUCT(BlueprintType)
struct FAppLauncherItemInformation
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Category;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTexture2D* Icon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool EnableMinimizeAndMaximize = true;

};

/**
 * Represents a Peacegate OS graphical program.
 */
UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UPeacegateProgramAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName ExecutableName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FAppLauncherItemInformation AppLauncherItem;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UProgram> ProgramClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool IsUnlockedByDefault = false;
};
