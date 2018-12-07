// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "TerminalCommand.h"
#include "TerminalCommandParserLibrary.h"
#include "CommandInfo.h"
#include "CommonUtils.h"
#include "UConsoleContext.h"
#include "UPeacegateFileSystem.h"
#include "USystemContext.h"
#include "DocoptForUnrealBPLibrary.h"
#include "UCommandProcessor.h"
#include "UBashCommand.generated.h"

/**
 * A Peacegate terminal command that acts as a pseudo-bash interpreter.
 */
UCLASS(BlueprintType)
class PROJECTOGLOWIA_API UBashCommand : public UTerminalCommand
{
	GENERATED_BODY()

private:
	UPROPERTY()
		TArray<FCommandRunInstruction> NextCommands;

	UPROPERTY()
	UConsoleContext* StartConsoleContext;

	UPROPERTY()
	UConsoleContext* CurrentConsoleContext;

	UPROPERTY()
	UConsoleContext* LastConsoleContext;

protected:
	UFUNCTION()
	void ProcessNextCommand();

public:
	virtual void NativeRunCommand(UConsoleContext* InConsole, const TMap<FString, UDocoptValue*> InArguments) override;
};
