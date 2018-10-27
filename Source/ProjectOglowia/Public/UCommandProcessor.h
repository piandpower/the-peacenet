// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "LatentActions.h"
#include "CoreMinimal.h"
#include "UConsoleContext.h"
#include "DocoptForUnrealBPLibrary.h"
#include "UCommandProcessor.generated.h"

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


UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UPiperContext : public UConsoleContext
{
	GENERATED_BODY()

public:
	UPiperContext* Input;
	UConsoleContext* Output;
	FString Log;
	
	virtual void Write(const FString& InText) override
	{
		if (Output)
		{
			Output->Write(InText);
		}
		else {
			Log += InText;
		}
	}

	virtual void WriteLine(const FString& InText) override { Write(InText + TEXT("\n")); }
	virtual void OverwriteLine(const FString& InText) override
	{
		if (Output)
		{
			Output->OverwriteLine(InText);
		}
		else {
			WriteLine(InText);
		}
	}

	virtual UConsoleContext* CreateChildContext(USystemContext* InSystemContext, int InUserID) override
	{
		if (Output)
		{
			return Output->CreateChildContext(InSystemContext, InUserID);
		}
		else if (Input)
		{
			return Input->CreateChildContext(InSystemContext, InUserID);
		}

		return this;
	}

	virtual void Clear() override
	{
		if (Output)
			Output->Clear();
		else
			Log = TEXT("");
	}

	virtual void ReadLine(UObject* WorldContextObject, struct FLatentActionInfo LatentInfo, FString& OutText) override
	{
		if (Input)
		{
			int NewlineIndex = -1;
			if (Input->Log.FindChar(TEXT('\n'), NewlineIndex))
			{
				OutText = Input->Log.Left(NewlineIndex);
				Input->Log.RemoveAt(0, NewlineIndex + 1);
			}
			else {
				OutText = FString(Input->Log);
				Input->Log = TEXT("");
			}
			UWorld* world = WorldContextObject->GetWorld();
			if (world)
			{
				FLatentActionManager& LatentActionManager = world->GetLatentActionManager();
				if (LatentActionManager.FindExistingAction<FPlaceboLatentAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) == NULL)
				{

					//Here in a second, once I confirm the project loads, we need to see whats wrong with this
					LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, new FPlaceboLatentAction(LatentInfo));
				}
			}

		}
		else {
			Terminal->ReadLine(WorldContextObject, LatentInfo, OutText);
		}
	}
};

USTRUCT(BlueprintType)
struct PROJECTOGLOWIA_API FCommandRunInstruction
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	UTerminalCommand* Command;

	UPROPERTY(BlueprintReadOnly)
	TMap<FString, FDocoptValue> Arguments;

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
