// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "PTerminalWidget.h"
#include "CoreMinimal.h"
#include "DocoptForUnrealBPLibrary.h"
#include "UConsoleContext.h"
#include "USystemContext.h"
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

	UFUNCTION(BlueprintCallable, Category = "Terminal Command")
	virtual void RunCommand(UPARAM(Ref) UConsoleContext* InConsole, const TMap<FString, UDocoptValue*> InArguments);
protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Terminal Command")
	void OnRunCommand(const UConsoleContext* InConsole, const TMap<FString, UDocoptValue*>& InArguments);

	UFUNCTION(BlueprintCallable, Category="Terminal Command")
	void Complete() { Completed.Broadcast(); }
};

UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UAdminTerminalCommand : public UTerminalCommand
{
	GENERATED_BODY()

public:
	virtual void RunCommand(UConsoleContext* InConsole, const TMap<FString, UDocoptValue*> InArguments) override;
};

UCLASS()
class PROJECTOGLOWIA_API UHelpCommand : public UTerminalCommand
{
	GENERATED_BODY()

public:
	virtual void RunCommand(UConsoleContext* InConsole, const TMap<FString, UDocoptValue*> InArguments) override;
};