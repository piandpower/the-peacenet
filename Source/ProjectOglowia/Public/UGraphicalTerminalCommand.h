// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "TerminalCommand.h"
#include "UPeacegateProgramAsset.h"
#include "UGraphicalTerminalCommand.generated.h"

/**
 * Encapsulates a graphical Terminal Command that, when run, runs a graphical program as the user ID of the current console context.
 */
UCLASS()
class PROJECTOGLOWIA_API UGraphicalTerminalCommand : public UTerminalCommand
{
	GENERATED_BODY()

public:
	UGraphicalTerminalCommand();
	~UGraphicalTerminalCommand();

	UPROPERTY()
	UPeacegateProgramAsset* ProgramAsset;

	virtual void RunCommand(UConsoleContext* InConsole, const TMap<FString, UDocoptValue*> InArguments) override;
};
