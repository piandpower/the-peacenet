// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UBashCommand.h"
#include "Async.h"

void UBashCommand::ProcessNextCommand()
{
	if (this->NextCommands.Num())
	{
		this->LastConsoleContext = this->CurrentConsoleContext;
		auto RunInstruction = this->NextCommands[0];
		this->NextCommands.RemoveAt(0);
		this->CurrentConsoleContext = RunInstruction.IntendedContext;
		this->CurrentConsoleContext->WorkingDirectory = this->LastConsoleContext->WorkingDirectory;

		TScriptDelegate<> CompletedDelegate;
		CompletedDelegate.BindUFunction(this, "ProcessNextCommand");
		RunInstruction.Command->Completed.Add(CompletedDelegate);

		RunInstruction.Command->RunCommand(this->CurrentConsoleContext, RunInstruction.Arguments);
	}
	else
	{
		if (this->CurrentConsoleContext->IsA<URedirectedConsoleContext>())
		{
			auto Redirected = Cast<URedirectedConsoleContext>(this->CurrentConsoleContext);
			Redirected->DumpToFile(this->StartConsoleContext);
		}

		this->StartConsoleContext->WorkingDirectory = this->CurrentConsoleContext->WorkingDirectory;
		this->CurrentConsoleContext = nullptr;
		this->LastConsoleContext = nullptr;
		this->RunCommand(this->StartConsoleContext, TMap<FString, UDocoptValue*>());
	}
}

void UBashCommand::NativeRunCommand(UConsoleContext* InConsole, const TMap<FString, UDocoptValue*> InArguments)
{
	auto User = InConsole->SystemContext->GetUserInfo(InConsole->UserID);

	InConsole->Write("`8" + User.Username + "`1`*@`r`9" + InConsole->SystemContext->GetHostname() + "`1:" + InConsole->GetDisplayWorkingDirectory());
	if (User.IsAdminUser)
	{
		InConsole->Write("`*# `r");
	}
	else
	{
		InConsole->Write("`*$ `r");
	}

	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, InArguments, InConsole]()
	{
		FString UserInput = InConsole->SynchronouslyReadLine();
		AsyncTask(ENamedThreads::GameThread, [this, InConsole, InArguments, UserInput] {
			if (UserInput == "exit")
			{
				this->Complete();
				return;
			}
			if (UserInput.TrimStartAndEnd().IsEmpty())
			{
				this->RunCommand(InConsole, InArguments);
				return;
			}
			
			auto CommandsToRun = UCommandProcessor::ProcessCommand(InConsole, UserInput);

			if (CommandsToRun.Num() == 0)
			{
				this->RunCommand(InConsole, InArguments);
				return;
			}

			this->CurrentConsoleContext = InConsole;
			this->StartConsoleContext = InConsole;
			this->NextCommands = CommandsToRun;

			this->ProcessNextCommand();
		});
	});

}