// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "LatentActions.h"
#include "CoreMinimal.h"

/**
 * 
 */
class PROJECTOGLOWIA_API FTerminalSlowTypeLatentAction : public FPendingLatentAction
{
public:
	class UPTerminalWidget* TerminalWidgetInstance;
	FName ExecutionFunction;
	int32 OutputLink;
	FWeakObjectPtr CallbackTarget;
	TArray<TCHAR> TextToWrite;
	int CharIndex = 0;
	float Countdown;
	float CountdownTime;

	FTerminalSlowTypeLatentAction(UPTerminalWidget* InTerminal, const FLatentActionInfo& InLatentInfo, const FString& InText, float InCountdown)
		: TerminalWidgetInstance(InTerminal)
		, ExecutionFunction(InLatentInfo.ExecutionFunction)
		, OutputLink(InLatentInfo.Linkage)
		, CallbackTarget(InLatentInfo.CallbackTarget)
		, TextToWrite(InText.GetCharArray())
		, Countdown(InCountdown)
		, CountdownTime(InCountdown)
		{}

	virtual void UpdateOperation(FLatentResponse& Response) override;
};
