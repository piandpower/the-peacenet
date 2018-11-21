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

bool UPeacenetSaveGame::CompanyNameExists(FText CompanyName)
{
	for (auto& Company : Businesses)
	{
		if (Company.Name.EqualTo(CompanyName))
			return true;
	}
	return false;
}

bool UPeacenetSaveGame::DomainNameExists(FString InDomainName)
{
	return DomainNameMap.Contains(InDomainName);
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
	const float DISTANCE = 0.1f;

	for (auto& Company : this->Businesses)
	{
		if (Company.Country != InCountry)
			continue;

		float cX = Company.NodePosition.X;
		float cY = Company.NodePosition.Y;

		float x = InPosition.X;
		float y = InPosition.Y;

		if (FMath::IsNearlyEqual(x, cX, DISTANCE) && FMath::IsNearlyEqual(y, cY, DISTANCE))
		{
			return true;
		}
	}

	for (auto& Character : this->Characters)
	{
		if (Character.Country != InCountry)
			continue;

		float x = InPosition.X;
		float y = InPosition.Y;

		float cX = Character.NodePosition.X;
		float cY = Character.NodePosition.Y;

		if (FMath::IsNearlyEqual(x, cX, DISTANCE) && FMath::IsNearlyEqual(y, cY, DISTANCE))
		{
			return true;
		}
	}
	return false;
}
