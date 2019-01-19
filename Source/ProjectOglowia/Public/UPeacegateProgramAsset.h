// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "UWindow.h"
#include "USystemContext.h"
#include "Engine/DataAsset.h"
#include "UPeacegateProgramAsset.generated.h"

class UConsoleContext;
class UPTerminalWidget;
class APeacenetWorldStateActor;


USTRUCT(BlueprintType)
struct FAppLauncherItemInformation
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText Category;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTexture2D* Icon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool EnableMinimizeAndMaximize = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText Description;
};

/**
 * Represents a Peacegate OS graphical program.
 */
UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UPeacegateProgramAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Metadata")
	FName ExecutableName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Metadata")
	FAppLauncherItemInformation AppLauncherItem;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Program")
	TSubclassOf<UProgram> ProgramClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Metadata")
	bool IsUnlockedByDefault = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Program")
	TArray<FString> SupportedFileExtensions;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Program")
	bool IsSingleInstance = false;
};