#include "UHelpCommand.h"
#include "CommandInfo.h"
#include "UUserContext.h"

void UHelpCommand::NativeRunCommand(UConsoleContext* InConsole, const TMap<FString, UDocoptValue*> InArguments)
{
	InConsole->WriteLine(TEXT("&*&FCommand help&r&7"));
	InConsole->WriteLine(TEXT("------------------- \n"));

	TMap<FName, FString> CommandList;
	int MaxLength = 0;

	for (auto Program : InConsole->GetUserContext()->GetOwningSystem()->GetInstalledPrograms())
	{
		CommandList.Add(Program->ExecutableName, Program->AppLauncherItem.Description.ToString());
		int Length = Program->ExecutableName.ToString().GetCharArray().Num();
		if (Length > MaxLength)
		{
			MaxLength = Length;
		}
	}

	for (auto Command : InConsole->GetUserContext()->GetOwningSystem()->GetInstalledCommands())
	{
		if (!InConsole->GetUserContext()->GetPeacenet()->ManPages.Contains(Command->Info.CommandName))
			continue;
		FManPage ManPage = InConsole->GetUserContext()->GetPeacenet()->ManPages[Command->Info.CommandName];
		CommandList.Add(Command->Info.CommandName, ManPage.Description);
		int Length = Command->Info.CommandName.ToString().GetCharArray().Num();
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

		InConsole->WriteLine("&8&*" + NameStr + "&r&7: " + FString::ChrN(DistLength, TEXT(' ')) + CommandList[Name]);
	}
	this->Complete();
}
