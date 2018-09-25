// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UConsoleContext.h"
#include "SystemContext.h"

FString UConsoleContext::GetDisplayWorkingDirectory()
{
	FString UserHomeDirectory = ISystemContext::Execute_GetUserHomeDirectory(SystemContext.GetObject(), UserID);

	if (WorkingDirectory.StartsWith(UserHomeDirectory))
	{
		FString NewWorkingDirectory(WorkingDirectory);
		NewWorkingDirectory.RemoveFromStart(UserHomeDirectory);
		return TEXT("~") + NewWorkingDirectory;
	}
	return WorkingDirectory;
}
