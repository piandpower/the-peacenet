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

bool UPeacenetSaveGame::DomainNameExists(FString InDomainName)
{
	return DomainNameMap.Contains(InDomainName);
}

bool UPeacenetSaveGame::IPAddressAllocated(FString InIPAddress)
{
	for(auto IP : this->ComputerIPMap)
	{
		if(IP.Key == InIPAddress)
			return true;
	}
	return false;
}

void UPeacenetSaveGame::FixEntityIDs()
{
	TMap<int, int> ComputerIDMap;
	TMap<int, int> CharacterIDMap;

	// Set the IDs of every computer to match their array index.
	for(int i = 0; i < Computers.Num(); i++)
	{
		ComputerIDMap.Add(Computers[i].ID, i);
		Computers[i].ID = i;
	}

	// Fix up character IDs and reassign their computer IDs
	for(int i = 0; i < Characters.Num(); i++)
	{
		CharacterIDMap.Add(Characters[i].ID, i);
		Characters[i].ID = i;
		Characters[i].ComputerID = ComputerIDMap[Characters[i].ComputerID];
	}

	// Fix up player character ID
	PlayerCharacterID = CharacterIDMap[PlayerCharacterID];

	// Fix up IP addresses.
	TArray<FString> IPsToRemove;
	for(auto& IPAddress : ComputerIPMap)
	{
		if(ComputerIDMap.Contains(IPAddress.Value))
		{
			IPAddress.Value = ComputerIDMap[IPAddress.Value];
		}
		else
		{
			IPsToRemove.Add(IPAddress.Key);
		}
	}
	while(IPsToRemove.Num())
	{
		ComputerIPMap.Remove(IPsToRemove[0]);
		IPsToRemove.RemoveAt(0);
	}

	TArray<int> RelationshipsToRemove;

	// Now we fix up character relationships.
	for(int i = 0; i < CharacterRelationships.Num(); i++)
	{
		FCharacterRelationship& Relationship = this->CharacterRelationships[i];
		if(!(CharacterIDMap.Contains(Relationship.FirstEntityID) && CharacterIDMap.Contains(Relationship.SecondEntityID)))
		{
			RelationshipsToRemove.Add(i);
			continue;
		}

		Relationship.FirstEntityID = CharacterIDMap[Relationship.FirstEntityID];
		Relationship.SecondEntityID = CharacterIDMap[Relationship.SecondEntityID];
	}

	while(RelationshipsToRemove.Num())
	{
		CharacterRelationships.RemoveAt(RelationshipsToRemove[0]);
		RelationshipsToRemove.RemoveAt(0);
		for(int i = 0; i < RelationshipsToRemove.Num(); i++)
		{
			RelationshipsToRemove[i]--;
		}
	}
}

bool UPeacenetSaveGame::RelatesWith(int InFirstEntity, int InSecondEntity)
{
	for(auto& Relationship : this->CharacterRelationships)
	{
		if(Relationship.FirstEntityID == InFirstEntity && Relationship.SecondEntityID == InSecondEntity)
			return true;
		if(Relationship.SecondEntityID == InFirstEntity && Relationship.FirstEntityID == InSecondEntity)
			return true;	
	}
	return false;
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