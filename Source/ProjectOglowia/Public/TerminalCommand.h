// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "PTerminalWidget.h"
#include "CoreMinimal.h"
#include "TerminalCommand.generated.h"
/**
 * 
 */
UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UTerminalCommand : public UObject
{
	GENERATED_BODY()

public:
	UTerminalCommand();
	~UTerminalCommand();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Terminal Command")
	void OnRunCommand(const UPTerminalWidget* InConsole);
};
