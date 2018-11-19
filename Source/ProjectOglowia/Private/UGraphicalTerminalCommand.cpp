// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UGraphicalTerminalCommand.h"
#include "PeacenetWorldStateActor.h"

UGraphicalTerminalCommand::UGraphicalTerminalCommand()
{
}

UGraphicalTerminalCommand::~UGraphicalTerminalCommand()
{
}

void UGraphicalTerminalCommand::RunCommand(UConsoleContext * InConsole, const TMap<FString, UDocoptValue*> InArguments)
{
	if (!InConsole->SystemContext->Desktop)
	{
		InConsole->WriteLine("error: cannot connect to Xorg server (Is Peacegate Desktop running?)");
		this->Complete();
		return;
	}

	UWindow* OutputWindow = nullptr;

	UProgram* Program = UProgram::CreateProgram(InConsole->SystemContext->Peacenet->WindowClass, this->ProgramAsset->ProgramClass, InConsole->SystemContext, InConsole->UserID, OutputWindow);

	OutputWindow->WindowTitle = this->ProgramAsset->AppLauncherItem.Name;
	OutputWindow->Icon = this->ProgramAsset->AppLauncherItem.Icon;
	OutputWindow->EnableMinimizeAndMaximize = this->ProgramAsset->AppLauncherItem.EnableMinimizeAndMaximize;

	this->Complete();
}
