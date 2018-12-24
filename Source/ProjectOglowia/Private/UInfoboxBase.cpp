#include "UInfoboxBase.h"
#include "UWindow.h"

void UInfoboxBase::BindCallbacks(const FInfoboxDismissedEvent InDismissedEvent, const FInfoboxInputValidator InTextValidator)
{
	DismissCallback = InDismissedEvent;
	Validator = InTextValidator;
}


void UInfoboxBase::Dismiss(const EDialogResult InDialogResult, const FText& InUserInputText)
{
	Window->Close();
	DismissCallback.ExecuteIfBound(InDialogResult, InUserInputText);
}


void UInfoboxBase::ValidateTextInput(const FText& InText)
{
	if (Validator.IsBound())
	{
		Validator.Execute(InText, TextInputErrorText);
	}
	else
	{
		TextInputErrorText = FText::GetEmpty();
	}
}
