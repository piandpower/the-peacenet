// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UGraphicalTerminalCommand.h"
#include "UProgram.h"
#include "UUserContext.h"
#include "PeacenetWorldStateActor.h"

UGraphicalTerminalCommand::UGraphicalTerminalCommand()
{
}

UGraphicalTerminalCommand::~UGraphicalTerminalCommand()
{
}

void UGraphicalTerminalCommand::NativeRunCommand(UConsoleContext * InConsole, const TMap<FString, UDocoptValue*> InArguments)
{
	if (!InConsole->GetUserContext()->GetDesktop())
	{
		InConsole->WriteLine("error: cannot connect to Xorg server (Is Peacegate Desktop running?)");
		this->Complete();
		return;
	}

	UProgram* OpenedProgram = nullptr;
	InConsole->GetUserContext()->OpenProgram(this->ProgramAsset->ExecutableName, OpenedProgram, false);

	this->Complete();
}
