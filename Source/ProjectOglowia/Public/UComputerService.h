// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UComputerTypeAsset.h"
#include "UHackableAsset.h"
#include "EHardnessValue.h"
#include "Text.h"
#include "UComputerService.generated.h"

/**
 * Represents a service that can run on a Peacenet computer. Contains info about compatible computer types, rarity, minimum/maximum skill levels, and what vulnerabilities are exposed on this service.
 */
UCLASS(Blueprintable, BlueprintType)
class PROJECTOGLOWIA_API UComputerService : public UDataAsset
{
	GENERATED_BODY()

public:
	// The ID of this asset when referenced in the save file.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Metadata")
	FName InternalID;

	// The base hackable asset for this service. When this service is hacked, the selected hackable will determine what kind of access the hacker gets to the hacked system.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Metadata")
	UHackableAsset* HackableType;

	// How hard is it, generally, to crack this service? This value will determine how severe a hack of this service is.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill & Rarity")
	EHardnessValue Hardness = EHardnessValue::Easy;

	// The minimum skill level this service can spawn at. 0 = no boundary.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill & Rarity")
	int MinimumSkillLevel = 0;

	// The maximum skill level this service can spawn at. 0 = no boundary.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill & Rarity")
	int MaximumSkillLevel = 0;



};
