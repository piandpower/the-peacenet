// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UNetMapWidget.h"
#include "UPeacenetSaveGame.h"
#include "PeacenetworldStateActor.h"
#include "UDesktopWidget.h"

void UNetMapWidget::CollectDiscoveredNodes()
{
	// Clear all existing nodes.
	this->Nodes.Empty();

	this->NodesCleared();

	// What we do depends on the display type.
	if (this->DisplayType == ENetMapDisplayType::CountryMap)
	{
		for (auto& Character : this->Desktop->SystemContext->Peacenet->SaveGame->Characters)
		{
			if (Character.Country != this->Desktop->SystemContext->Character.Country)
				continue;

			for (auto& Computer : this->Desktop->SystemContext->Peacenet->SaveGame->Computers)
			{
				if (Computer.ID == Character.ComputerID)
				{
					FNetMapNode Node;
					Node.EntityID = Character.ID;
					Node.EmblemType = ENetMapEmblemType::Character;
					Node.ColorValue = Character.Reputation;
					Node.Location = this->CalculateLocation(Character, Computer);
					Nodes.Add(Node);
					NodeAdded(Node);
				}
			}
		}
	}
}

FVector2D UNetMapWidget::CalculateLocation(const FPeacenetIdentity & InCharacter, const FComputer & InComputer)
{
	return InCharacter.NodePosition;
}

void UNetMapWidget::NativeConstruct()
{
	// First time we collect our nodes from the save file.
	this->CollectDiscoveredNodes();
}