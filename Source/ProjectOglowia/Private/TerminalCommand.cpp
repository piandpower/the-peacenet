// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "TerminalCommand.h"
#include "PeacenetWorldStateActor.h"

UTerminalCommand::UTerminalCommand()
{
}

UTerminalCommand::~UTerminalCommand()
{
}void UTerminalCommand::RunCommand(UConsoleContext* InConsole, const TMap<FString, UDocoptValue*> InArguments)
{
	OnRunCommand(InConsole, InArguments);
}

void UHelpCommand::RunCommand(UConsoleContext* InConsole, const TMap<FString, UDocoptValue*> InArguments)
{
	InConsole->WriteLine(TEXT("`*`1Command help`r"));
	InConsole->WriteLine(TEXT("------------------- \n"));

	TMap<FName, FString> CommandList;
	int MaxLength = 0;

	for (auto Program : InConsole->SystemContext->Peacenet->Programs)
	{
		if (InConsole->SystemContext->Computer.InstalledPrograms.Contains(Program->ExecutableName))
		{
			CommandList.Add(Program->ExecutableName, Program->AppLauncherItem.Description.ToString());
			int Length = Program->ExecutableName.ToString().GetCharArray().Num();
			if (Length > MaxLength)
			{
				MaxLength = Length;
			}
		}
	}

	for (auto Command : InConsole->SystemContext->Computer.InstalledCommands)
	{
		if (!InConsole->SystemContext->Peacenet->ManPages.Contains(Command))
			continue;
		FManPage ManPage = InConsole->SystemContext->Peacenet->ManPages[Command];
		CommandList.Add(Command, ManPage.Description);
		int Length = Command.ToString().GetCharArray().Num();
		if (Length > MaxLength)
		{
			MaxLength = Length;
		}
	}

	TArray<FName> CommandNames;
	CommandList.GetKeys(CommandNames);

	for (auto Name : CommandNames)
	{
		FString NameStr = Name.ToString();
		int DistLength = (MaxLength + 2) - (NameStr.GetCharArray().Num() + 2);

		InConsole->WriteLine(TEXT("`8`*") + NameStr + TEXT("`r`1: ") + FString::ChrN(DistLength, TEXT(' ')) + CommandList[Name]);
	}
	this->Complete();
}

void UAdminTerminalCommand::RunCommand(UConsoleContext* InConsole, const TMap<FString, UDocoptValue*> InArguments)
{
	if (InConsole->SystemContext->GetUserDomain(InConsole->UserID) != EUserDomain::Administrator)
	{
		InConsole->WriteLine(TEXT("error: must be run as root."));
		this->Complete();
		return;
	}

	Super::RunCommand(InConsole, InArguments);
}