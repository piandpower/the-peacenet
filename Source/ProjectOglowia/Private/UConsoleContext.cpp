// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UConsoleContext.h"
#include "CommonUtils.h"
#include "UUserContext.h"
#include "USystemContext.h"

void UConsoleContext::Setup(UUserContext* InUserContext)
{
	// Crash if we're not being given a valid user context.
	check(InUserContext);

	// Crash if we already have a user context.
	check(!this->GetUserContext());

	// Set our user context.
	this->UserContext = InUserContext;
}

FString UConsoleContext::GetWorkingDirectory()
{
	return this->WorkingDirectory;
}

UUserContext* UConsoleContext::GetUserContext()
{
	return this->UserContext;
}

UPTerminalWidget* UConsoleContext::GetTerminal()
{
	return this->Terminal;
}

void UConsoleContext::SetTerminal(UPTerminalWidget* InTerminalWidget)
{
	check(InTerminalWidget);
	this->Terminal = InTerminalWidget;
}

void UConsoleContext::InjectInput(const FString & Input)
{
	if (this->Terminal)
	{
		this->Terminal->InjectInput(Input);
	}
}

FString UConsoleContext::SynchronouslyReadLine()
{
	while (!this->GetTerminal()->IsInputLineAvailable) {	} //this is thread safe woo
	this->Terminal->IsInputLineAvailable = false;
	FString Input = this->Terminal->GetInputText();
	if (Input.EndsWith("\n"))
	{
		Input.RemoveFromEnd("\n");
	}
	this->Terminal->ClearInput();
	return Input;
}

UConsoleContext * UConsoleContext::CreateChildContext(USystemContext* InSystemContext, int InUserID)
{
	// Create a new console context owned by us.
	UConsoleContext* NewCtx = NewObject<UConsoleContext>(this);

	// Give it our user context
	NewCtx->Setup(this->GetUserContext());

	// Set the working directory to that of our home.
	NewCtx->WorkingDirectory = this->GetUserContext()->GetHomeDirectory();

	// Set the terminal to ours.
	NewCtx->SetTerminal(this->GetTerminal());

	// Done.
	return NewCtx;
}

void UConsoleContext::SetWorkingDirectory(const FString & InPath)
{
	if (this->GetUserContext()->GetFilesystem()->DirectoryExists(InPath))
	{
		this->WorkingDirectory = InPath;
	}
}

FString UConsoleContext::CombineWithWorkingDirectory(const FString & InPath)
{
	if (InPath.StartsWith("/"))
		return this->GetUserContext()->GetFilesystem()->ResolveToAbsolute(InPath);
	return this->GetUserContext()->GetFilesystem()->ResolveToAbsolute(WorkingDirectory + TEXT("/") + InPath);
}

FString UConsoleContext::GetDisplayWorkingDirectory()
{
	if (WorkingDirectory.StartsWith(this->GetUserContext()->GetHomeDirectory()))
	{
		FString NewWorkingDirectory(WorkingDirectory);
		NewWorkingDirectory.RemoveFromStart(this->GetUserContext()->GetHomeDirectory());
		return TEXT("~") + NewWorkingDirectory;
	}
	return WorkingDirectory;
}

void UConsoleContext::MakeBold()
{
	Write("&*");
}

void UConsoleContext::MakeBoldItalic()
{
	Write("&-");
}

void UConsoleContext::MakeItalic()
{
	Write("&_");
}

void UConsoleContext::ResetFormatting()
{
	Write("&r");
}

void UConsoleContext::SetAttention()
{
	Write("&!");
}

void UConsoleContext::InvertColors()
{
	Write("&~");
}

void UConsoleContext::SetColor(ETerminalColor InColor)
{
	Write(UCommonUtils::GetTerminalColorCode(InColor));
}

void UConsoleContext::ReadLine(UObject* WorldContextObject, FLatentActionInfo LatentInfo, FString& OutText)
{
	this->GetTerminal()->ReadLine(this->GetUserContext()->GetPeacenet(), LatentInfo, OutText); 
}
