// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "PTerminalWidget.h"
#include "CoreMinimal.h"
#include "DocoptForUnrealBPLibrary.h"
#include "UConsoleContext.h"
#include "GameFramework/Actor.h"
#include "USystemContext.h"
#include "TerminalCommand.generated.h"

class UUserContext;
class UAddressBookContext;
class UCommandInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCommandCompletedEvent);

/**
 * A simple Unreal object containing functions needed to run a Peacegate OS Terminal command.
 */
UCLASS(Blueprintable)
class PROJECTOGLOWIA_API ATerminalCommand : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UConsoleContext* Console;

public:
	UPROPERTY(BlueprintReadOnly, Category = "Terminal Command")
	UCommandInfo* CommandInfo;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FCommandCompletedEvent Completed;

	UFUNCTION(BlueprintCallable, Category = "Terminal Command")
	void RunCommand(UPARAM(Ref) UConsoleContext* InConsole, const TMap<FString, UDocoptValue*> InArguments);

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Terminal Command")
	UConsoleContext* GetConsole();

protected:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Terminal Command")
	UUserContext* GetUserContext();

	virtual void NativeRunCommand(UConsoleContext* InConsole, const TMap<FString, UDocoptValue*> InArguments);

	UFUNCTION(BlueprintImplementableEvent, Category = "Terminal Command")
	void OnRunCommand(const UConsoleContext* InConsole, const TMap<FString, UDocoptValue*>& InArguments);

	UFUNCTION(BlueprintCallable, Category = "Terminal Command")
	void Complete();
};