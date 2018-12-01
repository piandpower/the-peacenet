// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UCharacterDisplayBase.h"
#include "PeacenetWorldStateActor.h"
#include "UPeacenetSaveGame.h"

void UCharacterDisplayBase::SetCharacterIdChecked(int InEntityID)
{
	check(this->Window->SystemContext->Peacenet->SaveGame->GetCharacterByID(InEntityID, this->Character));

	for (auto& SaveComputer : this->Window->SystemContext->Peacenet->SaveGame->Computers)
	{
		if (SaveComputer.ID == Character.ComputerID)
		{
			this->Computer = SaveComputer;
		}
	}
}

void UCharacterDisplayBase::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	// Is this identity already pinned?
	this->IsPinned = this->Window->SystemContext->Peacenet->SaveGame->IsEntityKnown(this->Character.ID, EPinnedContactType::Person);

	Super::NativeTick(MyGeometry, InDeltaTime);
}
