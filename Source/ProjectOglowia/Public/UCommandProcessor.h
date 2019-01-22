// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "LatentActions.h"
#include "CoreMinimal.h"
#include "UConsoleContext.h"
#include "DocoptForUnrealBPLibrary.h"
#include "UCommandProcessor.generated.h"

class UPiperContext;
class URedirectedConsoleContext;
class UTerminalCommand;

USTRUCT(BlueprintType)
struct PROJECTOGLOWIA_API FCommandRunInstruction
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	ATerminalCommand* Command;

	UPROPERTY(BlueprintReadOnly)
	TMap<FString, UDocoptValue*> Arguments;

	UPROPERTY(BlueprintReadOnly)
	UConsoleContext* IntendedContext;

};
/**
 * 
 */
UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UCommandProcessor : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Bash")
	static TArray<FCommandRunInstruction> ProcessCommand(UPARAM(Ref) UConsoleContext* InConsole, const FString& InCommand);
};
