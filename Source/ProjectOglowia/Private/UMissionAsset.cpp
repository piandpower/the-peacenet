// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UMissionAsset.h"
#include "UMissionUnlock.h"
#include "UUnlockAction.h"

bool UMissionAsset::GetUnlocks(TArray<UMissionUnlock*>& OutUnlocks)
{
	for (auto ActionInfo : this->Actions)
	{
		auto Action = ActionInfo.Action;
		if (Action->IsA<UUnlockAction>())
		{
			UUnlockAction* UnlockAction = Cast<UUnlockAction>(Action);
			OutUnlocks.Add(UnlockAction->Unlock.Unlock);
		}
	}

	return OutUnlocks.Num() > 0;
}
