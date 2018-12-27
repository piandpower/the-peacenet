// Copyright (c) 2018 Alkaline Thunder & The Peacenet.

#pragma once

#include "CoreMinimal.h"
#include "TerminalCommand.h"
#include "UConsoleContext.h"
#include "DocoptForUnrealBPLibrary.h"
#include "UHelpCommand.generated.h"

UCLASS()
class PROJECTOGLOWIA_API UHelpCommand : public UTerminalCommand
{
	GENERATED_BODY()

protected:
	virtual void NativeRunCommand(UConsoleContext* InConsole, const TMap<FString, UDocoptValue*> InArguments) override;
};