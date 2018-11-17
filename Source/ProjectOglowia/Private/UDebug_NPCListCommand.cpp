// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UDebug_NPCListCommand.h"
#include "Math.h"
#include "CommonUtils.h"

void UDebug_NPCListCommand::RunCommand(UConsoleContext* InConsole, const TMap<FString, UDocoptValue*> InArguments)
{
	FString UserCommand = InArguments["<command>"]->AsString();

	if (UserCommand == "npcs")
	{
		InConsole->WriteLine("`8`*Characters`1`r\n===============\n");
		InConsole->WriteLine("`*ID\t\tName\t\t\t\tComputer ID\t\tReputation\t\tCountry\t\tSkill`r");
		for (auto Character : InConsole->SystemContext->Peacenet->SaveGame->Characters)
		{
			if (Character.CharacterType == EIdentityType::Player)
				continue;

			InConsole->Write(FString::FromInt(Character.ID) + "\t\t");
			InConsole->Write(Character.CharacterName.ToString() + "\t\t\t\t");
			InConsole->Write(FString::FromInt(Character.ComputerID) + "\t\t");
			if (FMath::IsNearlyEqual(Character.Reputation, 0, 0.1f))
			{
				InConsole->Write("`*Neutral`r");
			}
			else if (Character.Reputation < 0)
			{
				InConsole->Write("`3`*Malicious`1`r");
			}
			else
			{
				InConsole->Write("`8`*Heroic`1`r");
			}
			InConsole->Write("\t\t");
			InConsole->Write(FString::FromInt((int)Character.Country) + "\t\t");
			InConsole->WriteLine(FString::FromInt(Character.Skill));
		}
	}
	else if (UserCommand == "computers")
	{
		InConsole->WriteLine("Computers\n================\n");
		InConsole->WriteLine("ID\t\tIP Addr\t\tType\t\t# Users\t\t# Dirs");
		for (auto Computer : InConsole->SystemContext->Peacenet->SaveGame->Computers)
		{
			InConsole->Write(FString::FromInt(Computer.ID) + "\t\t");
			InConsole->Write(Computer.IPAddress + "\t\t");
			InConsole->Write(Computer.ComputerType.ToString() + "\t\t");
			InConsole->Write(FString::FromInt(Computer.Users.Num()) + "\t\t");
			InConsole->WriteLine(FString::FromInt(Computer.Filesystem.Num()));
		}
	}
	else
	{
		InConsole->WriteLine("Unrecognized command: " + UserCommand);
	}
	this->Complete();
}