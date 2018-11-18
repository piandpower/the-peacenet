// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "TerminalCommand.h"
#include "UConsoleContext.h"
#include "DocoptForUnrealBPLibrary.h"
#include "USystemContext.h"
#include "UDebug_SSHCommand.generated.h"

/**
 * A debug command to simulate an in-game ssh connection with another computer.
 */
UCLASS(BlueprintType)
class PROJECTOGLOWIA_API UDebug_SSHCommand : public UTerminalCommand
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UTerminalCommand* ChildBashCommand;

	FString ChildUsage;
	FString ChildFriendlyUsage;

	UPROPERTY()
	UConsoleContext* CurrentContext;

public:
	virtual void RunCommand(UConsoleContext* InConsole, TMap<FString, UDocoptValue*> InArguments) override;
};
