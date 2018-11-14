// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UCheckSaveBoolean.h"

void UCheckSaveBoolean::NativeTick(float InDeltaTime)
{
	if (!SaveGame->Booleans.Contains(this->BooleanName))
	{
		SaveGame->Booleans.Add(this->BooleanName, false);
	}

	if (this->SaveGame->Booleans[this->BooleanName] == this->ExpectedResult)
	{
		this->Complete();
	}
}
