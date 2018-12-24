// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UNetMapWidget.h"
#include "UPeacenetSaveGame.h"
#include "PeacenetWorldStateActor.h"
#include "UDesktopWidget.h"

void UNetMapWidget::CollectDiscoveredNodes()
{
	// Clear all existing nodes.
	this->Nodes.Empty();

	this->NodesCleared();

	// What we do depends on the display type.
	if (this->DisplayType == ENetMapDisplayType::CountryMap)
	{
		for (auto& Contact : this->Desktop->SystemContext->Peacenet->SaveGame->PinnedContacts)
		{
			if (Contact.ContactType == EPinnedContactType::Person)
			{
				// personal
				FPeacenetIdentity Character;
				if (this->Desktop->SystemContext->Peacenet->SaveGame->GetCharacterByID(Contact.EntityID, Character))
				{
					if (Character.Country == this->Desktop->SystemContext->Character.Country)
					{
						FNetMapNode CharNode;
						CharNode.ColorValue = Character.Reputation;
						CharNode.Contact = Contact;

						Nodes.Add(CharNode);
						NodeAdded(CharNode);
					}
				}
			}
			else
			{
				// business
				FEnterpriseNetwork Company;
				if (this->Desktop->SystemContext->Peacenet->SaveGame->GetBusinessByID(Contact.EntityID, Company))
				{
					if (Company.Country == this->Desktop->SystemContext->Character.Country)
					{
						FNetMapNode BusinessNode;
						BusinessNode.ColorValue = 0.f;
						BusinessNode.Contact = Contact;

						Nodes.Add(BusinessNode);
						NodeAdded(BusinessNode);
					}
				}
			}
		}
	}
}

void UNetMapWidget::SelectNode(const FNetMapNode& InNode)
{
	switch (InNode.Contact.ContactType)
	{
	case EPinnedContactType::Person:
		this->Desktop->SelectCharacterNode(InNode.Contact.EntityID);
		break;
	}
}

FVector2D UNetMapWidget::CalculateLocation(const FPeacenetIdentity & InCharacter, const FComputer & InComputer)
{
	return InCharacter.NodePosition;
}

bool UNetMapWidget::GetCharacterData(const FNetMapNode & InNode, FPeacenetIdentity & OutCharacter)
{
	if (InNode.Contact.ContactType == EPinnedContactType::Person)
	{
		return this->Desktop->SystemContext->Peacenet->SaveGame->GetCharacterByID(InNode.Contact.EntityID, OutCharacter);
	}
	return false;
}

void UNetMapWidget::NativeConstruct()
{
	// First time we collect our nodes from the save file.
	this->CollectDiscoveredNodes();
}