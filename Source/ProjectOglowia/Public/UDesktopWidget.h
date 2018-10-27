// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "UserWidget.h"
#include "UPeacegateProgramAsset.h" 
#include "UDesktopWidget.generated.h"

class USystemContext;

/**
 * Base class of a Desktop environment.
 */
UCLASS(Blueprintable, Abstract)
class PROJECTOGLOWIA_API UDesktopWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	USystemContext * SystemContext;

	UPROPERTY(BlueprintReadOnly)
	int UserID = 0;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Desktop")
	void ShowProgramOnWorkspace(const UProgram* InProgram);
};
