// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "TerminalCommand.h"
#include "USystemContext.h"
#include "DocoptForUnrealBPLibrary.h"
#include "PeacenetWorldStateActor.h"
#include "UDebug_NPCListCommand.generated.h"

/**
 * A Terminal Command that lists all hackable NPCs.
 */
UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UDebug_NPCListCommand : public UTerminalCommand
{
	GENERATED_BODY()

protected:
	virtual void NativeRunCommand(UConsoleContext* InConsole, const TMap<FString, UDocoptValue*> InArguments) override;
};
