// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UDebug_SSHCommand.h"
#include "PeacenetWorldStateActor.h"

void UDebug_SSHCommand::RunCommand(UConsoleContext* InConsole, TMap<FString, UDocoptValue*> InArguments)
{
	InConsole->WriteLine("[debug] skipping ssh authentication and going straight to connection as non-root...");

	FString IPAddress = InArguments["<host>"]->AsString();

	USystemContext* ResolvedSystemContext;
	FString ResolvedIP;

	if (InConsole->SystemContext->Peacenet->ResolveHost(IPAddress, ResolvedIP, ResolvedSystemContext))
	{
		UConsoleContext* ChildContext = InConsole->CreateChildContext(ResolvedSystemContext, 1);
		
		TScriptDelegate<> OnCompleted;
		OnCompleted.BindUFunction(this, "Complete");
	
		ResolvedSystemContext->TryGetTerminalCommand("bash", this->ChildBashCommand, this->ChildUsage, this->ChildFriendlyUsage);
		this->ChildBashCommand->Completed.Add(OnCompleted);

		this->CurrentContext = InConsole;

		this->ChildBashCommand->RunCommand(ChildContext, TMap<FString, UDocoptValue*>());
	}
	else
	{
		InConsole->WriteLine("ssh: cannot connect to host.");
		this->Complete();
	}
}