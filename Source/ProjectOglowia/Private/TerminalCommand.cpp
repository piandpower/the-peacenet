// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "TerminalCommand.h"
#include "CommandInfo.h"
#include "PeacenetWorldStateActor.h"
#include "UAddressBookContext.h"

UAddressBookContext* UTerminalCommand::GetAddressBook()
{
	return this->Console->SystemContext->GetAddressBook();
}

UConsoleContext* UTerminalCommand::GetConsole()
{
	return this->Console;
}

UTerminalCommand::UTerminalCommand()
{
}

UTerminalCommand::~UTerminalCommand()
{
}

void UTerminalCommand::RunCommand(UConsoleContext* InConsole, const TMap<FString, UDocoptValue*> InArguments)
{
	this->Console = InConsole;

	if(this->CommandInfo)
	{
		this->Console->SystemContext->LogEvent(this->Console->UserID, "ran " + this->CommandInfo->Info.CommandName.ToString());
	}
	
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

void UHelpCommand::NativeRunCommand(UConsoleContext* InConsole, const TMap<FString, UDocoptValue*> InArguments)
{
	InConsole->WriteLine(TEXT("&*&ACommand help&r&1"));
	InConsole->WriteLine(TEXT("------------------- \n"));

	TMap<FName, FString> CommandList;
	int MaxLength = 0;

	for (auto Program : InConsole->SystemContext->GetPeacenet()->Programs)
	{
		if (InConsole->SystemContext->GetComputer().InstalledPrograms.Contains(Program->ExecutableName))
		{
			CommandList.Add(Program->ExecutableName, Program->AppLauncherItem.Description.ToString());
			int Length = Program->ExecutableName.ToString().GetCharArray().Num();
			if (Length > MaxLength)
			{
				MaxLength = Length;
			}
		}
	}

	for (auto Command : InConsole->SystemContext->GetComputer().InstalledCommands)
	{
		if (!InConsole->SystemContext->GetPeacenet()->ManPages.Contains(Command))
			continue;
		FManPage ManPage = InConsole->SystemContext->GetPeacenet()->ManPages[Command];
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

void UAdminTerminalCommand::NativeRunCommand(UConsoleContext* InConsole, const TMap<FString, UDocoptValue*> InArguments)
{
	if (InConsole->SystemContext->GetUserDomain(InConsole->UserID) != EUserDomain::Administrator)
	{
		InConsole->WriteLine(TEXT("error: must be run as root."));
		this->Complete();
		return;
	}

	Super::NativeRunCommand(InConsole, InArguments);
}