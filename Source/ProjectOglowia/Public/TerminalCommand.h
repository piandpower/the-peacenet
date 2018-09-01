// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "PTerminalWidget.h"
#include "CoreMinimal.h"
#include "DocoptForUnrealBPLibrary.h"
#include "UConsoleContext.h"
#include "CommandSeeker.h"
#include "SystemContext.h"
#include "TerminalCommand.generated.h"

/**
 * A simple Unreal object containing functions needed to run a Peacegate OS Terminal command.
 */
UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UTerminalCommand : public UObject
{
	GENERATED_BODY()

		DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCommandCompletedEvent);
public:
	UTerminalCommand();
	~UTerminalCommand();

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FCommandCompletedEvent Completed;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Terminal Command")
	void OnRunCommand(const UConsoleContext* InConsole, const TScriptInterface<ICommandSeeker>& InSeeker, const TScriptInterface<ISystemContext>& InSystemContext, const TMap<FString, FDocoptValue>& InArguments);

protected:
	UFUNCTION(BlueprintCallable, Category="Terminal Command")
	void Complete() { Completed.Broadcast(); }
};
