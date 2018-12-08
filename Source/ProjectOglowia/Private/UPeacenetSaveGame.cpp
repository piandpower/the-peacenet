// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UPeacenetSaveGame.h"

bool UPeacenetSaveGame::CharacterNameExists(FString CharacterName)
{
	for (auto& Character : Characters)
	{
		if (Character.CharacterName == CharacterName)
		{
			return true;
		}
	}
	return false;
}

bool UPeacenetSaveGame::CompanyNameExists(FString CompanyName)
{
	for (auto& Company : Businesses)
	{
		if (Company.Name == CompanyName)
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

		if(FVector2D::Distance(InPosition, Company.NodePosition) <= DISTANCE)
		{
			return true;
		}
	}

	for (auto& Character : this->Characters)
	{
		if (Character.Country != InCountry)
			continue;

		if (FVector2D::Distance(InPosition, Character.NodePosition) <= DISTANCE) 
		{
			return true;
		}
	}
	return false;
}

bool UPeacenetSaveGame::CountryHasEmailService(ECountry InCountry)
{
	// TODO: What is an email service?
	return true;
}

bool UPeacenetSaveGame::GetCharacterByID(int InEntityID, FPeacenetIdentity & OutCharacter)
{
	for (auto& Character : this->Characters)
	{
		if (Character.ID == InEntityID)
		{
			OutCharacter = Character;
			return true;
		}
	}
	return false;
}

bool UPeacenetSaveGame::GetBusinessByID(int InEntityID, FEnterpriseNetwork& OutCompany)
{
	for (auto& Company : this->Businesses)
	{
		if (Company.ID == InEntityID)
		{
			OutCompany = Company;
			return true;
		}
	}
	return false;
}

bool UPeacenetSaveGame::IsEntityKnown(int InEntityID, EPinnedContactType InContactType)
{
	for (auto& Contact : this->PinnedContacts)
	{
		if (Contact.EntityID == InEntityID && Contact.ContactType == InContactType)
			return true;
	}

	return false;
}
