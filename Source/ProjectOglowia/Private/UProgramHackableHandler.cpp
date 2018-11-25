// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UProgramHackableHandler.h"
#include "PeacenetWorldStateActor.h"

bool UProgramHackableHandler::CanHandle(USystemContext * InCaller)
{
	check(this->ProgramToRun); //crash dev builds if we didn't specify a program.

	// Non-dev builds will simply not allow you to hack the service.
	if (!this->ProgramToRun)
		return false;

	// We can handle if the caller has the program installed OR the program is unlocked by default.
	return InCaller->Computer.InstalledPrograms.Contains(this->ProgramToRun->ExecutableName) || ProgramToRun->IsUnlockedByDefault;
}

void UProgramHackableHandler::Handle(USystemContext * InCaller, USystemContext * InTarget)
{
	// TODO: WE REALLY SHOULDN'T BE USING THE ROOT USER FOR THIS
	int UserID = 0;

	// Normally, we let the program opener handle setting up the program's contexts and adding it to the screen, but we need to do that ourselves so that the program is visible to the caller but is running on the target.
	UWindow* ProgramWindow;
	UProgram* Program = UProgram::CreateProgram(InCaller->Peacenet->WindowClass, this->ProgramToRun->ProgramClass, InTarget, UserID, ProgramWindow, false);

	Program->SetupContexts();
	InCaller->ShowWindowOnWorkspace(Program);

	ProgramWindow->WindowTitle = this->ProgramToRun->AppLauncherItem.Name;
	ProgramWindow->Icon = this->ProgramToRun->AppLauncherItem.Icon;
}
