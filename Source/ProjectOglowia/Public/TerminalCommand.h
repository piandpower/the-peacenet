// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "PTerminalWidget.h"
#include "CoreMinimal.h"
#include "DocoptBlueprintAPI.h"
#include "TerminalCommand.generated.h"
/**
 * A simple Unreal object containing functions needed to run a Peacegate OS Terminal command.
 */
UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UTerminalCommand : public UObject
{
	GENERATED_BODY()

public:
	UTerminalCommand();
	~UTerminalCommand();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Terminal Command")
	void OnRunCommand(const UPTerminalWidget* InConsole, const TMap<FString, FDocoptValue>& InArguments);
};
