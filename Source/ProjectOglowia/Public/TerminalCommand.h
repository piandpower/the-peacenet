// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "PTerminalWidget.h"
#include "CoreMinimal.h"
#include "DocoptForUnrealBPLibrary.h"
#include "UConsoleContext.h"
#include "USystemContext.h"
#include "TerminalCommand.generated.h"

class UAddressBookContext;
class UCommandInfo;

/**
 * A simple Unreal object containing functions needed to run a Peacegate OS Terminal command.
 */
UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UTerminalCommand : public UObject
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCommandCompletedEvent);

protected:
	UFUNCTION(BlueprintCallable, BlueprintCallable, Category = "Address Book")
	UAddressBookContext* GetAddressBook();

public:
	UTerminalCommand();
	~UTerminalCommand();

	UPROPERTY(BlueprintReadOnly, Category = "Terminal Command")
	UCommandInfo* CommandInfo;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FCommandCompletedEvent Completed;

	UFUNCTION(BlueprintCallable, Category = "Terminal Command")
	void RunCommand(UPARAM(Ref) UConsoleContext* InConsole, const TMap<FString, UDocoptValue*> InArguments);

private:
	UPROPERTY()
	UConsoleContext* Console;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Terminal Command")
	UConsoleContext* GetConsole();

protected:

	virtual void NativeRunCommand(UConsoleContext* InConsole, const TMap<FString, UDocoptValue*> InArguments);

	UFUNCTION(BlueprintImplementableEvent, Category = "Terminal Command")
	void OnRunCommand(const UConsoleContext* InConsole, const TMap<FString, UDocoptValue*>& InArguments);

	UFUNCTION(BlueprintCallable, Category = "Terminal Command")
	void Complete();
};

UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UAdminTerminalCommand : public UTerminalCommand
{
	GENERATED_BODY()

protected:
	virtual void NativeRunCommand(UConsoleContext* InConsole, const TMap<FString, UDocoptValue*> InArguments) override;
};

UCLASS()
class PROJECTOGLOWIA_API UHelpCommand : public UTerminalCommand
{
	GENERATED_BODY()

protected:
	virtual void NativeRunCommand(UConsoleContext* InConsole, const TMap<FString, UDocoptValue*> InArguments) override;
};