// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UConsoleContext.h"
#include "SystemContext.h"

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
