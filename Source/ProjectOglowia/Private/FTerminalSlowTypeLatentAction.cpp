// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "FTerminalSlowTypeLatentAction.h"

void FTerminalSlowTypeLatentAction::UpdateOperation(FLatentResponse & Response)
{
	if (CharIndex >= TextToWrite.Num())
	{
		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
		return;
	}

	Countdown = Countdown - Response.ElapsedTime();
	if (Countdown <= 0)
	{
		Countdown = CountdownTime;
		TCHAR Character = TextToWrite[CharIndex];
		TerminalWidgetInstance->Write(FString::Chr(Character));
		CharIndex++;
	}
}
