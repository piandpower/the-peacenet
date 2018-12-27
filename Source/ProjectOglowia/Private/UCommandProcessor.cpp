// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UCommandProcessor.h"
#include "USystemContext.h"
#include "CommonUtils.h"
#include "URedirectedConsoleContext.h"
#include "UPiperContext.h"
#include "UUserContext.h"
#include "TerminalCommand.h"
#include "TerminalCommandParserLibrary.h"


TArray<FCommandRunInstruction> UCommandProcessor::ProcessCommand(UConsoleContext* InConsole, const FString& InCommand)
{
	check(InConsole);

	TArray<FCommandRunInstruction> CommandsToRun;

	FString TrimmedCommand = InCommand.TrimStartAndEnd();

	if (TrimmedCommand.IsEmpty())
		return CommandsToRun;

	FString OutError;

	FPeacegateCommandInstruction Instruction = UTerminalCommandParserLibrary::GetCommandList(InCommand, InConsole->GetUserContext()->GetHomeDirectory(), OutError);

	

	if (!OutError.IsEmpty())
	{
		InConsole->WriteLine(OutError);
		return CommandsToRun;
	}

	int CommandNum = Instruction.Commands.Num();

	if (CommandNum == 0)
		return CommandsToRun;

	if (!Instruction.OutputFile.IsEmpty())
	{
		if (InConsole->Filesystem->DirectoryExists(Instruction.OutputFile))
		{
			InConsole->WriteLine("`3`*error: " + Instruction.OutputFile + ": Directory exists.`1`r");
			return CommandsToRun;
		}
	}

	UPiperContext* LastPiper = nullptr;

	for (int i = 0; i < CommandNum; i++)
	{
		bool IsPiping = (i < CommandNum - 1);
	
		FString CommandText = Instruction.Commands[i];

		FString Home;
		if (LastPiper)
		{
			Home = LastPiper->HomeDirectory;
		}
		else {
			Home = InConsole->HomeDirectory;
		}

		FString TokenError;

		TArray<FString> Tokens = UTerminalCommandParserLibrary::Tokenize(CommandText, Home, TokenError);

		if (!TokenError.IsEmpty())
		{
			InConsole->WriteLine(TokenError);
			return TArray<FCommandRunInstruction>();
		}

		FString Name = Tokens[0];
		FString InternalUsage;
		FString FriendlyUsage;
		Tokens.RemoveAt(0);



		UTerminalCommand* CommandImpl = nullptr;
		if (InConsole->SystemContext->TryGetTerminalCommand(FName(*Name), CommandImpl, InternalUsage, FriendlyUsage))
		{
			bool DocoptError = false;
			FString DocoptErrorMessage;

			TMap<FString, UDocoptValue*> Args = UDocoptForUnrealBPLibrary::NativeParseArguments(InternalUsage, Tokens, true, TEXT(""), false, DocoptError, DocoptErrorMessage);
			
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
					Ctx->HomeDirectory = LastPiper->HomeDirectory;
					Ctx->UserID = LastPiper->UserID;
					Ctx->SystemContext = LastPiper->SystemContext;
				}
				else
				{
					Ctx->HomeDirectory = InConsole->HomeDirectory;
					Ctx->UserID = InConsole->UserID;
					Ctx->SystemContext = InConsole->SystemContext;
				}

				Ctx->Filesystem = Ctx->SystemContext->GetFilesystem(Ctx->UserID);
				
				NewInst.IntendedContext = Ctx;
				LastPiper = Ctx;
			}
			else
			{
				UPiperContext* Ctx = nullptr;
				if (Instruction.OutputFile.IsEmpty())
				{
					Ctx = NewObject<UPiperContext>();
					Ctx->Output = InConsole;
				}
				else
				{
					Ctx = NewObject<UPiperContext>(InConsole, URedirectedConsoleContext::StaticClass());
					auto Redirected = Cast<URedirectedConsoleContext>(Ctx);
					Redirected->OutputFilePath = Instruction.OutputFile;
					Redirected->Overwrite = Instruction.Overwrites;

				}
				Ctx->Input = LastPiper;
				if (LastPiper)
				{
					Ctx->HomeDirectory = LastPiper->HomeDirectory;
					Ctx->UserID = LastPiper->UserID;
					Ctx->SystemContext = LastPiper->SystemContext;
				}
				else
				{
					Ctx->HomeDirectory = InConsole->HomeDirectory;
					Ctx->UserID = InConsole->UserID;
					Ctx->SystemContext = InConsole->SystemContext;
				}

				Ctx->Filesystem = Ctx->SystemContext->GetFilesystem(Ctx->UserID);

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