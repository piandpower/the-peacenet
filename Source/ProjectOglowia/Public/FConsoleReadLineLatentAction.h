#pragma once

#include "LatentActions.h"
#include "Blueprint/UserWidget.h"
#include "UserWidget.h" //not the same?
#include "PTerminalWidget.h"

class FConsoleReadLineLatentAction : public FPendingLatentAction
{
public:
	class UPTerminalWidget* TerminalWidgetInstance;
	FName ExecutionFunction;
	int32 OutputLink;
	FWeakObjectPtr CallbackTarget;
	FString& OutputText;

	FConsoleReadLineLatentAction(UPTerminalWidget* InTerminal, const FLatentActionInfo& InLatentInfo, FString& OutText)
		: TerminalWidgetInstance(InTerminal)
		, ExecutionFunction(InLatentInfo.ExecutionFunction)
		, OutputLink(InLatentInfo.Linkage)
		, CallbackTarget(InLatentInfo.CallbackTarget)
		, OutputText(OutText) {}

	virtual void UpdateOperation(FLatentResponse& Response) override
	{
		OutputText = TerminalWidgetInstance->GetInputText();
		if (OutputText.EndsWith("\n")) 
		{
			OutputText.RemoveFromEnd(TEXT("\n"));
			Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
			TerminalWidgetInstance->ClearInput();
		}
	}
};

