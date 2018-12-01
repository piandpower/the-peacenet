// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UCharacterDisplayBase.h"
#include "PeacenetWorldStateActor.h"
#include "UPeacenetSaveGame.h"

void UCharacterDisplayBase::SetCharacterIdChecked(int InEntityID)
{
	check(this->Window->SystemContext->Peacenet->SaveGame->GetCharacterByID(InEntityID, this->Character));
}
