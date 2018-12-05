// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UWorkspace.h"
#include "UPeacegateProgramAsset.h"

void UWorkspace::OnWindowClosed(UWindow * InWindow)
{
	if (InWindow == ActiveWindow)
	{
		ActiveWindow = nullptr;
	}
}

void UWorkspace::ShowProgramOnWorkspace(UProgram * InProgram)
{
	UWindow* Window = InProgram->Window;
	TScriptDelegate<> WindowFocus;
	WindowFocus.BindUFunction(this, "WindowFocusEvent");
	Window->WindowFocusEvent.Add(WindowFocus);
	TScriptDelegate<> WindowClosed;
	WindowClosed.BindUFunction(this, "OnWindowClosed");
	Window->NativeWindowClosed.Add(WindowClosed);
	this->OnWindowAdded(Window);
}

void UWorkspace::WindowFocusEvent(bool IsFocused, UWindow * InWindow)
{
	if (IsFocused)
	{
		this->ActiveWindow = InWindow;
	}
	else
	{
		if (this->ActiveWindow == InWindow)
		{
			this->ActiveWindow = nullptr;
		}
	}
}
