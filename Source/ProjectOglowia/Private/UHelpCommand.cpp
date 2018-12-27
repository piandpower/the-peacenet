#include "UHelpCommand.h"
#include "UUserContext.h"

void UHelpCommand::NativeRunCommand(UConsoleContext* InConsole, const TMap<FString, UDocoptValue*> InArguments)
{
	InConsole->WriteLine(TEXT("&*&ACommand help&r&1"));
	InConsole->WriteLine(TEXT("------------------- \n"));

	TMap<FName, FString> CommandList;
	int MaxLength = 0;

	for (auto Program : InConsole->GetUserContext()->GetPeacenet()->Programs)
	{
		if (InConsole->GetUserContext()->GetOwningSystem()->GetComputer().InstalledPrograms.Contains(Program->ExecutableName))
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
