// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UWindow.h"

// Show an infobox (no callbacks.)
void UWindow::ShowInfo(const FText& InTitle, const FText& InMessage, const EInfoboxIcon InIcon)
{
	FInfoboxDismissedEvent Dismissed;
	FInfoboxInputValidator Validator;

	this->ShowInfoWithCallbacks(InTitle, InMessage, InIcon, EInfoboxButtonLayout::OK, false, Dismissed, Validator);
}

void UWindow::Close()
{
	OnWindowClosed();
	NativeWindowClosed.Broadcast(this);
}

void UWindow::Minimize()
{
	OnWindowMinimized();
}

void UWindow::Maximize()
{
	OnWindowMaximized();
}

void UWindow::Restore()
{
	OnWindowRestored();
}

void UWindow::AddWindowToClientSlot(const UUserWidget* InClientWidget)
{
	OnAddWindowToClientSlot(InClientWidget);
}

void UWindow::SetClientMinimumSize(const FVector2D& InSize)
{
	OnSetClientMinimumSize(InSize);
}

void UWindow::NativeOnAddedToFocusPath(const FFocusEvent & InFocusEvent)
{
	this->WindowFocusEvent.Broadcast(true, this);
	Super::NativeOnAddedToFocusPath(InFocusEvent);
}

void UWindow::NativeOnRemovedFromFocusPath(const FFocusEvent & InFocusEvent)
{
	this->WindowFocusEvent.Broadcast(false, this);
	Super::NativeOnRemovedFromFocusPath(InFocusEvent);
}
