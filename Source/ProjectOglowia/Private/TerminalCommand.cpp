// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "TerminalCommand.h"

UTerminalCommand::UTerminalCommand()
{
}

UTerminalCommand::~UTerminalCommand()
{
}void UTerminalCommand::RunCommand(UConsoleContext* InConsole, const TMap<FString, FDocoptValue> InArguments)
{
	TScriptInterface<ICommandSeeker> InSeeker = ISystemContext::Execute_GetCommandSeeker(InConsole->SystemContext.GetObject());

	OnRunCommand(InConsole, InSeeker, InArguments);
}

void UAdminTerminalCommand::RunCommand(UConsoleContext* InConsole, const TMap<FString, FDocoptValue> InArguments)
{
	if (ISystemContext::Execute_GetUserDomain(InConsole->SystemContext.GetObject(), InConsole->UserID) != EUserDomain::Administrator)
	{
		InConsole->WriteLine(TEXT("error: must be run as root."));
		this->Complete();
		return;
	}

	Super::RunCommand(InConsole, InArguments);
}