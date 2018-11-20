// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UPeacenetSaveGame.h"

bool UPeacenetSaveGame::CharacterNameExists(FText CharacterName)
{
	for (auto& Character : Characters)
	{
		if (Character.CharacterName.EqualTo(CharacterName))
		{
			return true;
		}
	}
	return false;
}

bool UPeacenetSaveGame::IPAddressAllocated(FString InIPAddress)
{
	for (auto& Computer : this->Computers)
	{
		if (Computer.IPAddress == InIPAddress)
		{
			return true;
		}
	}
	return false;
}

bool UPeacenetSaveGame::IsCharacterNodePositionTaken(ECountry InCountry, FVector2D InPosition)
{
	for (auto& Character : this->Characters)
	{
		if (Character.Country != InCountry)
			continue;

		float x = InPosition.X;
		float y = InPosition.Y;

		float cX = Character.NodePosition.X;
		float cY = Character.NodePosition.Y;

		if (FMath::IsNearlyEqual(x, cX, 0.1f) && FMath::IsNearlyEqual(y, cY, 0.1f))
		{
			return true;
		}
	}
	return false;
}
