// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CommandSeeker.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCommandSeeker : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTOGLOWIA_API ICommandSeeker
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintImplementableEvent, Category="Peacegate")
	class UTerminalCommand* FindCommand(const FString& InCommandName, FString& OutInternalUsage, FString& OutFriendlyUsage);	

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Peacegate")
	TMap<FName, FString> GetCommandsWithDescriptions();
};
