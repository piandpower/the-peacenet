// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UConsoleContext.h"
#include "SystemContext.h"

UConsoleContext * UConsoleContext::CreateChildContext(TScriptInterface<ISystemContext> InSystemContext, int InUserID)
{
	UConsoleContext* NewCtx = NewObject<UConsoleContext>();

	NewCtx->SystemContext = InSystemContext;
	NewCtx->UserID = InUserID;

	NewCtx->WorkingDirectory = ISystemContext::Execute_GetUserHomeDirectory(InSystemContext.GetObject(), InUserID);
	NewCtx->Filesystem = ISystemContext::Execute_GetFilesystem(InSystemContext.GetObject(), InUserID);

	NewCtx->Terminal = this->Terminal;

	return NewCtx;
}

FString UConsoleContext::GetHostname()
{
	return ISystemContext::Execute_GetHostname(this->SystemContext.GetObject());
}

FString UConsoleContext::GetUsername()
{
	FUserInfo User = ISystemContext::Execute_GetUserInfo(this->SystemContext.GetObject(), this->UserID);
	return User.Username;
}

FString UConsoleContext::GetUserTypeDisplay()
{
	FUserInfo User = ISystemContext::Execute_GetUserInfo(this->SystemContext.GetObject(), this->UserID);
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
