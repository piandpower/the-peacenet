// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UCommandProcessor.h"
#include "TerminalCommandParserLibrary.h"

TArray<FCommandRunInstruction> UCommandProcessor::ProcessCommand(TScriptInterface<ICommandSeeker> InCommandSeeker, UConsoleContext* InConsole, const FString& InCommand)
{
	TArray<FCommandRunInstruction> CommandsToRun;

	FString TrimmedCommand = InCommand.TrimStartAndEnd();

	if (TrimmedCommand.IsEmpty())
		return CommandsToRun;

	FString OutError;

	FPeacegateCommandInstruction Instruction = UTerminalCommandParserLibrary::GetCommandList(InCommand, OutError);

	if (!OutError.IsEmpty())
	{
		InConsole->WriteLine(OutError);
		return CommandsToRun;
	}

	int CommandNum = Instruction.Commands.Num();

	if (CommandNum == 0)
		return CommandsToRun;

	UPiperContext* LastPiper = nullptr;

	for (int i = 0; i < CommandNum; i++)
	{
		bool IsPiping = (i < CommandNum - 1);
	
		FString CommandText = Instruction.Commands[i];

		FString TokenError;

		TArray<FString> Tokens = UTerminalCommandParserLibrary::Tokenize(CommandText, TokenError);

		if (!TokenError.IsEmpty())
		{
			InConsole->WriteLine(TokenError);
			return TArray<FCommandRunInstruction>();
		}

		FString Name = Tokens[0];
		FString InternalUsage;
		FString FriendlyUsage;
		Tokens.RemoveAt(0);



		UTerminalCommand* CommandImpl = ICommandSeeker::Execute_FindCommand(InCommandSeeker.GetObject(), Name, InternalUsage, FriendlyUsage);
		if (CommandImpl)
		{
			bool DocoptError = false;
			FString DocoptErrorMessage;

			TMap<FString, FDocoptValue> Args = UDocoptForUnrealBPLibrary::NativeParseArguments(InternalUsage, Tokens, true, TEXT(""), false, DocoptError, DocoptErrorMessage);
			
			if (DocoptError)
			{
				InConsole->WriteLine(DocoptErrorMessage);
				return TArray<FCommandRunInstruction>();
			}

			FCommandRunInstruction NewInst;
			NewInst.Arguments = Args;
			NewInst.Command = CommandImpl;

			if (IsPiping)
			{
				UPiperContext* Ctx = NewObject<UPiperContext>();
				Ctx->Input = LastPiper;
				if (LastPiper)
				{
					Ctx->UserID = LastPiper->UserID;
					Ctx->SystemContext = LastPiper->SystemContext;
				}
				else
				{
					Ctx->UserID = InConsole->UserID;
					Ctx->SystemContext = InConsole->SystemContext;
				}
				
				NewInst.IntendedContext = Ctx;
				LastPiper = Ctx;
			}
			else
			{
				UPiperContext* Ctx = NewObject<UPiperContext>();
				Ctx->Input = LastPiper;
				Ctx->Output = InConsole;
				if (LastPiper)
				{
					Ctx->UserID = LastPiper->UserID;
					Ctx->SystemContext = LastPiper->SystemContext;
				}
				else
				{
					Ctx->UserID = InConsole->UserID;
					Ctx->SystemContext = InConsole->SystemContext;
				}
				NewInst.IntendedContext = Ctx;
			}

			CommandsToRun.Add(NewInst);
		}
		else 
		{
			InConsole->WriteLine(Name + TEXT(": Command not found."));
			return TArray<FCommandRunInstruction>();
		}
	}

	



	return CommandsToRun;
}