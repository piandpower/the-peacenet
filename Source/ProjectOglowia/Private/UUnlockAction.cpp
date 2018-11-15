// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UUnlockAction.h"
#include "PeacenetWorldStateActor.h"


void UUnlockAction::NativeExecuteMissionAction()
{
	if (Unlock.Unlock)
	{
		Unlock.Unlock->PerformUnlock(this->SystemContext, this->SaveGame);
		this->SystemContext->Peacenet->MissionUnlocks.Add(Unlock.Unlock);
	}
}