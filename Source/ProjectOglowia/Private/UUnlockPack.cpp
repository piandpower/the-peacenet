// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UUnlockPack.h"

void UUnlockPack::PerformUnlock(USystemContext* InSystemContext, UPeacenetSaveGame* InSaveGame)
{
	for (auto Child : this->ChildUnlocks)
	{
		if (Child.Unlock)
		{
			Child.Unlock->PerformUnlock(InSystemContext, InSaveGame);
		}
	}
}