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

bool UPeacenetSaveGame::GetCharacterByID(int InEntityID, FPeacenetIdentity & OutCharacter, int& OutIndex)
{
	int min = 0;
	int max = this->Characters.Num() - 1;

	int average = (min + max) / 2;

	while((max - min) >= 0)
	{
		FPeacenetIdentity& Character = Characters[average];

		if(Character.ID == InEntityID)
		{
			OutCharacter = Character;
			OutIndex = average;
			return true;
		}
		else if(Character.ID < InEntityID)
		{
			min = average + 1;
			average = (min + max) / 2;
		}
		else
		{
			max = average - 1;
			average = (min + max) / 2;
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

bool UPeacenetSaveGame::GetComputerByID(int InEntityID, FComputer& OutComputer, int& OutIndex)
{
	int min = 0;
	int max = this->Computers.Num() - 1;

	int average = (min + max) / 2;

	while((max - min) >= 0)
	{
		FComputer& Computer = Computers[average];

		if(Computer.ID == InEntityID)
		{
			OutComputer = Computer;
			OutIndex = average;
			return true;
		}
		else if(Computer.ID < InEntityID)
		{
			min = average + 1;
			average = (min + max) / 2;
		}
		else
		{
			max = average - 1;
			average = (min + max) / 2;
		}
	}

	return false;
}