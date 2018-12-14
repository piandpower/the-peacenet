// Copyright (c) 2018 Alkaline Thunder & The Peacenet.

#pragma once

#include "CoreMinimal.h"
#include "TerminalCommand.h"
#include "UConsoleContext.h"
#include "DocoptForUnrealBPLibrary.h"
#include "ULogScraperCommand.generated.h"

/**
 * Performs scraping of the stdin stream for passwords.
 */
UCLASS(BlueprintType)
class PROJECTOGLOWIA_API ULogScraperCommand : public UTerminalCommand
{
    GENERATED_BODY()

public:
    virtual void NativeRunCommand(UConsoleContext* InConsole, const TMap<FString, UDocoptValue*> InArguments) override;
};