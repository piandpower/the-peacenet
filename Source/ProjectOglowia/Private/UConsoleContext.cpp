// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UConsoleContext.h"
#include "USystemContext.h"

FString UConsoleContext::SynchronouslyReadLine()
{
	while (!this->Terminal->IsInputLineAvailable) {	} //this is thread safe woo
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
	UConsoleContext* NewCtx = NewObject<UConsoleContext>();

	NewCtx->SystemContext = InSystemContext;
	NewCtx->UserID = InUserID;

	NewCtx->HomeDirectory = InSystemContext->GetUserHomeDirectory(InUserID);
	NewCtx->WorkingDirectory = NewCtx->HomeDirectory;
	NewCtx->Filesystem = InSystemContext->GetFilesystem(InUserID);

	NewCtx->Terminal = this->Terminal;

	return NewCtx;
}

FString UConsoleContext::GetHostname()
{
	return this->SystemContext->GetHostname();
}

FString UConsoleContext::GetUsername()
{
	FUserInfo User = this->SystemContext->GetUserInfo(this->UserID);
	return User.Username;
}

FString UConsoleContext::GetUserTypeDisplay()
{
	FUserInfo User = this->SystemContext->GetUserInfo(this->UserID);
	return User.IsAdminUser ? TEXT("#") : TEXT("$");
}

void UConsoleContext::SetWorkingDirectory(const FString & InPath)
{
	if (Filesystem->DirectoryExists(InPath))
	{
		WorkingDirectory = InPath;
	}
}

FString UConsoleContext::CombineWithWorkingDirectory(const FString & InPath)
{
	if (InPath.StartsWith("/"))
		return Filesystem->ResolveToAbsolute(InPath);
	return Filesystem->ResolveToAbsolute(WorkingDirectory + TEXT("/") + InPath);
}

FString UConsoleContext::GetDisplayWorkingDirectory()
{
	if (WorkingDirectory.StartsWith(HomeDirectory))
	{
		FString NewWorkingDirectory(WorkingDirectory);
		NewWorkingDirectory.RemoveFromStart(HomeDirectory);
		return TEXT("~") + NewWorkingDirectory;
	}
	return WorkingDirectory;
}
