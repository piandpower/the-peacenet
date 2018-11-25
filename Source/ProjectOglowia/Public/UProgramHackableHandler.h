// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "UHackableHandler.h"
#include "UPeacegateProgramAsset.h"
#include "UWindow.h"
#include "UDesktopWidget.h"
#include "UProgramHackableHandler.generated.h"

/**
 * Runs a program as the hacked system if the program is installed/unlocked on the caller.
 */
UCLASS(BlueprintType, EditInlineNew)
class PROJECTOGLOWIA_API UProgramHackableHandler : public UHackableHandler
{
	GENERATED_BODY()

public:
	// This is the program that is run when the target system is successfully hacked.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Program")
	UPeacegateProgramAsset* ProgramToRun;

	virtual bool CanHandle(USystemContext* InCaller) override;
	virtual void Handle(USystemContext* InCaller, USystemContext* InTarget) override;
};
