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
	UWindow* OutputWindow = nullptr;

	UProgram* Program = UProgram::CreateProgram(InConsole->SystemContext->Peacenet, InConsole->SystemContext->Peacenet->WindowClass, this->ProgramAsset->ProgramClass, InConsole->SystemContext, InConsole->UserID, OutputWindow);

	OutputWindow->WindowTitle = FText::FromString(this->ProgramAsset->AppLauncherItem.Name);
	OutputWindow->Icon = this->ProgramAsset->AppLauncherItem.Icon;
	OutputWindow->EnableMinimizeAndMaximize = this->ProgramAsset->AppLauncherItem.EnableMinimizeAndMaximize;

	FScriptDelegate MyEvent;
	MyEvent.BindUFunction(this, TEXT("Complete"));

	OutputWindow->NativeWindowClosed.Add(MyEvent);
}
