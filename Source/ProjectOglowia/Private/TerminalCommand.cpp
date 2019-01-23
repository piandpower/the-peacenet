// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "TerminalCommand.h"
#include "CommandInfo.h"
#include "PeacenetWorldStateActor.h"
#include "UUserContext.h"

UUserContext* ATerminalCommand::GetUserContext()
{
	check(this->GetConsole());
	return this->GetConsole()->GetUserContext();
}

UConsoleContext* ATerminalCommand::GetConsole()
{
	return this->Console;
}

void ATerminalCommand::RunCommand(UConsoleContext* InConsole, const TMap<FString, UDocoptValue*> InArguments)
{
	this->Console = InConsole;	
	NativeRunCommand(InConsole, InArguments);
}

void ATerminalCommand::NativeRunCommand(UConsoleContext * InConsole, const TMap<FString, UDocoptValue*> InArguments)
{
	// Call into BP to do the rest.
	this->OnRunCommand(InConsole, InArguments);
}

void ATerminalCommand::Complete()
{
	this->Completed.Broadcast();
	
	// Despawn the actor and clean up memory used by the
	// terminal command. If we were a UObject we wouldn't
	// have to manually destroy ourselves, but then we
	// also wouldn't have access to "Delay" in Blueprint.
	this->Destroy();
}