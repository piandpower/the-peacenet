// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UTilerWorkspaceBase.h"
#include "WidgetTree.h"
#include "HorizontalBoxSlot.h"
#include "VerticalBoxSlot.h"

bool UTilerWorkspaceBase::IsBox(UPanelWidget * InWidget)
{
	if (Cast<UHorizontalBox>(InWidget))
		return true;
	if (Cast<UVerticalBox>(InWidget))
		return true;
	return false;
}

bool UTilerWorkspaceBase::RemoveEmptyBoxes(UPanelWidget * InWidget)
{
	if (InWidget->GetChildrenCount() == 0)
		return true;

	for (int i = 0; i < InWidget->GetChildrenCount(); i++)
	{
		UWidget* Child = InWidget->GetChildAt(i);

		if (!Cast<UPanelWidget>(Child))
			continue;

		bool result = this->RemoveEmptyBoxes(Cast<UPanelWidget>(Child));
		if (result)
		{
			Child->RemoveFromParent();
			i--;
			continue;

		}
	}

	return InWidget->GetChildrenCount() == 0;
}

void UTilerWorkspaceBase::NativeHandleOpenWindow(UWindow* InWindow)
{
	auto Player = this->GetOwningPlayer();

	// This is where we'll stick the window if we don't have an active window.
	UPanelWidget* DestPanel = this->GetTilerRootPanel();

	if (this->ActiveWindow)
	{
		// This is the panel that's holding the Active Window.
		auto ActiveParent = Cast<UPanelWidget>(this->ActiveWindow->GetParent());

		// This is the position in the parent's child list where this window resides. When splitting, this is extremely important.
		int ActiveWindowIndex = ActiveParent->GetChildIndex(ActiveWindow);

		if (this->CurrentSplitType == ETilerSplitType::Horizontal)
		{
			if (Cast<UHorizontalBox>(ActiveParent))
			{
				ActiveParent->AddChild(InWindow);

				Cast<UHorizontalBoxSlot>(InWindow->Slot)->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
			}
			else
			{
				UWindow* Backup = ActiveWindow; // In case the game takes focus away from the active window and it becomes nullptr as a result.
				ActiveWindow->RemoveFromParent();
				ActiveWindow = Backup;

				UHorizontalBox* SplitBox = this->WidgetTree->ConstructWidget<UHorizontalBox>();
				SplitBox->AddChild(ActiveWindow);
				SplitBox->AddChild(InWindow);

				Cast<UHorizontalBoxSlot>(ActiveWindow->Slot)->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
				Cast<UHorizontalBoxSlot>(InWindow->Slot)->SetSize(FSlateChildSize(ESlateSizeRule::Fill));

				ActiveParent->InsertChildAt(ActiveWindowIndex, SplitBox);
			
				// Set the split box fill if it was just added to a vertical box.
				if (Cast<UVerticalBox>(ActiveParent))
				{
					Cast<UVerticalBoxSlot>(SplitBox->Slot)->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
				}
			}
		}
		else
		{
			if (Cast<UVerticalBox>(ActiveParent))
			{
				ActiveParent->AddChild(InWindow);

				Cast<UVerticalBoxSlot>(InWindow->Slot)->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
			}
			else
			{
				UWindow* Backup = ActiveWindow; // In case the game takes focus away from the active window and it becomes nullptr as a result.
				ActiveWindow->RemoveFromParent();
				ActiveWindow = Backup;

				UVerticalBox* SplitBox = this->WidgetTree->ConstructWidget<UVerticalBox>();
				SplitBox->AddChild(ActiveWindow);
				SplitBox->AddChild(InWindow);

				Cast<UVerticalBoxSlot>(ActiveWindow->Slot)->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
				Cast<UVerticalBoxSlot>(InWindow->Slot)->SetSize(FSlateChildSize(ESlateSizeRule::Fill));

				ActiveParent->InsertChildAt(ActiveWindowIndex, SplitBox);

				// Set the split box fill if it was just added to a horizontal box.
				if (Cast<UHorizontalBox>(ActiveParent))
				{
					Cast<UHorizontalBoxSlot>(SplitBox->Slot)->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
				}
			}

		}
	}
	else
	{
		// We'll stick the window in the root panel. First we check if the root has children.
		if (DestPanel->GetChildrenCount())
		{
			// We'll grab the child widget.
			UWidget* Child = DestPanel->GetChildAt(0);

			// If it's a Window, then we need to wrap it in a box.
			if (UWindow* ChildWindow = Cast<UWindow>(Child))
			{
				Child->RemoveFromParent();

				UPanelWidget* NewParent = nullptr;
				if (this->CurrentSplitType == ETilerSplitType::Horizontal)
				{
					NewParent = this->WidgetTree->ConstructWidget<UHorizontalBox>();
					NewParent->AddChild(Child);
					NewParent->AddChild(InWindow);

					UHorizontalBoxSlot* ChildSlot = Cast<UHorizontalBoxSlot>(Child->Slot);
					UHorizontalBoxSlot* WinSlot = Cast<UHorizontalBoxSlot>(InWindow->Slot);

					ChildSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
					WinSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
				}
				else
				{
					NewParent = this->WidgetTree->ConstructWidget<UVerticalBox>();
					NewParent->AddChild(Child);
					NewParent->AddChild(InWindow);

					UVerticalBoxSlot* ChildSlot = Cast<UVerticalBoxSlot>(Child->Slot);
					UVerticalBoxSlot* WinSlot = Cast<UVerticalBoxSlot>(InWindow->Slot);

					ChildSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
					WinSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));

				}

				DestPanel->AddChild(NewParent);
			}
			else if (UPanelWidget* ChildPanel = Cast<UPanelWidget>(Child))
			{
				if (this->CurrentSplitType == ETilerSplitType::Vertical)
				{
					if (Cast<UVerticalBox>(ChildPanel))
					{
						ChildPanel->AddChild(InWindow);
					
						Cast<UVerticalBoxSlot>(InWindow->Slot)->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
					}
					else
					{
						ChildPanel->RemoveFromParent();
						UVerticalBox* NewParent = this->WidgetTree->ConstructWidget<UVerticalBox>();
						NewParent->AddChild(ChildPanel);
						NewParent->AddChild(InWindow);
						Cast<UVerticalBoxSlot>(ChildPanel->Slot)->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
						Cast<UVerticalBoxSlot>(InWindow->Slot)->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
						DestPanel->AddChild(NewParent);
					}
				}
				else
				{
					if (Cast<UHorizontalBox>(ChildPanel))
					{
						ChildPanel->AddChild(InWindow);

						Cast<UHorizontalBoxSlot>(InWindow->Slot)->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
					}
					else
					{
						ChildPanel->RemoveFromParent();
						UHorizontalBox* NewParent = this->WidgetTree->ConstructWidget<UHorizontalBox>();
						NewParent->AddChild(ChildPanel);
						NewParent->AddChild(InWindow);
						Cast<UHorizontalBoxSlot>(ChildPanel->Slot)->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
						Cast<UHorizontalBoxSlot>(InWindow->Slot)->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
						DestPanel->AddChild(NewParent);

					}

				}
			}
		}
		else
		{
			// No children, so we'll add the window right here.
			DestPanel->AddChild(InWindow);
		}
	}
}