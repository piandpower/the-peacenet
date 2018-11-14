 // Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "USetSaveBoolean.h"

void USetSaveBoolean::NativeExecuteMissionAction()
{
	if (this->SaveGame->Booleans.Contains(BooleanName))
	{
		this->SaveGame->Booleans[BooleanName] = NewValue;
	}
	else
	{
		this->SaveGame->Booleans.Add(BooleanName, NewValue);
	}
}
