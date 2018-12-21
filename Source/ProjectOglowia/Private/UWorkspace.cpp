// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UWorkspace.h"
#include "UPeacegateProgramAsset.h"

void UWorkspace::OnWindowClosed(UWindow * InWindow)
{
	if (InWindow == ActiveWindow)
	{
		ActiveWindow = nullptr;
	}

	int ProgramIndex = -1;
	for(int i = 0; i < this->Programs.Num(); i++)
	{
		UProgram* Program = this->Programs[i];
		if(Program->Window == InWindow)
		{
			ProgramIndex = i;
			break;
		}
	}

	if(ProgramIndex != -1)
	{
		this->Programs.RemoveAt(ProgramIndex);
	}
}

bool UWorkspace::HasExistingWindow(TSubclassOf<UProgram> InProgramClass, UProgram*& OutProgram)
{
	for(auto Program : this->Programs)
	{
		if(Program->GetClass() == InProgramClass)
		{
			OutProgram = Program;
			return true;
		}
	}
	return false;
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

	Programs.Add(InProgram);

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
