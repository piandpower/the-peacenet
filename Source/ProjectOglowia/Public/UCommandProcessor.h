// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "LatentActions.h"
#include "CoreMinimal.h"
#include "UConsoleContext.h"
#include "DocoptForUnrealBPLibrary.h"
#include "UCommandProcessor.generated.h"

class UPiperContext;
class UTerminalCommand;

class FPlaceboLatentAction : public FPendingLatentAction
{
public:
	FName ExecutionFunction;
	int32 OutputLink;
	FWeakObjectPtr CallbackTarget;

	FPlaceboLatentAction(const FLatentActionInfo& InLatentInfo)
		: ExecutionFunction(InLatentInfo.ExecutionFunction)
		, OutputLink(InLatentInfo.Linkage)
		, CallbackTarget(InLatentInfo.CallbackTarget) {}

	virtual void UpdateOperation(FLatentResponse& Response) override
	{
		Response.DoneIf(true);
	}
};

UCLASS()
class PROJECTOGLOWIA_API URedirectedConsoleContext : public UPiperContext
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString OutputFilePath;

	UPROPERTY()
	bool Overwrite = false;

	UFUNCTION(BlueprintCallable, Category = "Console Redirection")
	void DumpToFile(UConsoleContext* InConsole);


};

USTRUCT(BlueprintType)
struct PROJECTOGLOWIA_API FCommandRunInstruction
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	UTerminalCommand* Command;

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
