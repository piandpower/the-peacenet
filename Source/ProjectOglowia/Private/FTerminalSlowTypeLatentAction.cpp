// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "FTerminalSlowTypeLatentAction.h"
#include "PTerminalWidget.h"

void FTerminalSlowTypeLatentAction::UpdateOperation(FLatentResponse & Response)
{
	if (CharIndex >= TextToWrite.Num())
	{
		Response.DoneIf(true);
		return;
	}

	Countdown = Countdown - Response.ElapsedTime();
	if (Countdown <= 0)
	{
		Countdown = CountdownTime;
		TCHAR Character = TextToWrite[CharIndex];
		TerminalWidgetInstance->Write(FString::Chr(Character));
		CharIndex++;
		if (CharIndex < TextToWrite.Num())
		{
			if (Character == TEXT('`'))
			{
				uint8 dummy = 0;
				if (TextToWrite[CharIndex] == TEXT('`') || TerminalWidgetInstance->ParseEscape(TextToWrite[CharIndex], dummy, dummy))
				{
					TerminalWidgetInstance->Write(FString::Chr(TextToWrite[CharIndex]));
					CharIndex++;
				}
			}
		}
	}
}
