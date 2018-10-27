// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "TerminalCommand.h"
#include "PeacenetWorldStateActor.h"

UTerminalCommand::UTerminalCommand()
{
}

UTerminalCommand::~UTerminalCommand()
{
}void UTerminalCommand::RunCommand(UConsoleContext* InConsole, const TMap<FString, FDocoptValue> InArguments)
{
	OnRunCommand(InConsole, InArguments);
}

void UHelpCommand::RunCommand(UConsoleContext* InConsole, const TMap<FString, FDocoptValue> InArguments)
{
	InConsole->WriteLine(TEXT("`*`1Command help`r"));
	InConsole->WriteLine(TEXT("------------------- \n"));

	for (auto Command : InConsole->SystemContext->Computer.InstalledCommands)
	{
		InConsole->Write(TEXT("`8") + Command.ToString() + TEXT("`1\t:\t"));
		FManPage ManPage = InConsole->SystemContext->Peacenet->ManPages[Command];
		InConsole->WriteLine(ManPage.Description);
	}

	this->Complete();
}

void UAdminTerminalCommand::RunCommand(UConsoleContext* InConsole, const TMap<FString, FDocoptValue> InArguments)
{
	if (InConsole->SystemContext->GetUserDomain(InConsole->UserID) != EUserDomain::Administrator)
	{
		InConsole->WriteLine(TEXT("error: must be run as root."));
		this->Complete();
		return;
	}

	Super::RunCommand(InConsole, InArguments);
}