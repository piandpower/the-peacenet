// Copyright (c) 2019 Alkaline Thunder & Bit Phoenix Software.

#pragma once

#include "CoreMinimal.h"
#include "FComputer.h"
#include "FPeacenetIdentity.h"
#include "UConsoleContext.h"
#include "CommonUtils.h"
#include "TerminalCommand.h"
#include "UHackable.h"
#include "SecureShell.generated.h"

UCLASS(BlueprintType)
class PROJECTOGLOWIA_API USecureShell : public UHackable
{
    GENERATED_BODY()

private:
    UPROPERTY()
    ATerminalCommand* OwningCommand;

    UPROPERTY()
    FName Shell;

    UPROPERTY()
    UConsoleContext* ShellConsole;

public:
    UFUNCTION(BlueprintCallable, Category = "Secure Shell")
    void StartSecureShell(ATerminalCommand* InCaller, FName InShellCommand, FAuthenticationRequiredEvent InCallback);
};