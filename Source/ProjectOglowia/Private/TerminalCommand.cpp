// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "TerminalCommand.h"
#include "CommandInfo.h"
#include "PeacenetWorldStateActor.h"
#include "UUserContext.h"

UUserContext* UTerminalCommand::GetUserContext()
{
	check(this->GetConsole());
	return this->GetConsole()->GetUserContext();
}

UConsoleContext* UTerminalCommand::GetConsole()
{
	return this->Console;
}

void UTerminalCommand::RunCommand(UConsoleContext* InConsole, const TMap<FString, UDocoptValue*> InArguments)
{
	this->Console = InConsole;	
	NativeRunCommand(InConsole, InArguments);
}

void UTerminalCommand::NativeRunCommand(UConsoleContext * InConsole, const TMap<FString, UDocoptValue*> InArguments)
{
	// Call into BP to do the rest.
	this->OnRunCommand(InConsole, InArguments);
}

void UTerminalCommand::Complete()
{
	this->Completed.Broadcast();
}