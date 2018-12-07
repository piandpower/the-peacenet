// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "TerminalCommand.h"
#include "USystemContext.h"
#include "UComputerService.h"
#include "PeacenetWorldStateActor.h"
#include "DocoptForUnrealBPLibrary.h"
#include "UNmapCommand.generated.h"

/**
 * A Terminal Command that maps out the services running on a Peacenet computer/network.
 */
UCLASS(BlueprintType)
class PROJECTOGLOWIA_API UNmapCommand : public UTerminalCommand
{
	GENERATED_BODY()

public:
	UPROPERTY()
	USystemContext* ResolvedContext;

	UPROPERTY()
	TArray<FServiceInfo> Hackables;

	UPROPERTY()
	UConsoleContext* Caller;

	void ListNextService();

protected:
	virtual void NativeRunCommand(UConsoleContext* InConsole, const TMap<FString, UDocoptValue*> InArguments) override;
};
