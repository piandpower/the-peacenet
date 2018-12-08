// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "ECountry.h"
#include "UCompanyTypeAsset.h"
#include "FEnterpriseNetwork.generated.h"

/**
 * Represents a company in Peacenet's world, which is similar to a character (Peacenet Identity) but can own multiple computers and employ people.
 *
 * This is really more akin to a faction, to be honest. Maybe we could rename this come multiplayer?
 */
USTRUCT(BlueprintType)
struct PROJECTOGLOWIA_API FEnterpriseNetwork
{
	GENERATED_BODY()

public:
	// This is the entity ID for this entity.
	UPROPERTY(BlueprintReadOnly, Category = "Peacenet|Company")
	int ID = 0;

	// Where is this company?
	UPROPERTY(BlueprintReadOnly, Category = "Peacenet|Company")
	ECountry Country;

	// Where in the netmap is this company?
	UPROPERTY()
	FVector2D NodePosition;

	// This is the name of the company, displayed in the UI. No two companies should have the same name unless in different countries. If it does have the same name in a different country, both companies inherit similar stats.
	UPROPERTY(BlueprintReadOnly, Category = "Peacenet|Company")
	FString Name;

	// A list of all computers the company owns. Each element is an entity ID (see FComputer.h).
	UPROPERTY(BlueprintReadOnly, Category = "Peacenet|Company")
	TArray<int> Computers;

	// Info about the type of company this is.
	UPROPERTY()
	TSubclassOf<UCompanyTypeAsset> CompanyType;

	UPROPERTY()
	FString PublicIPAddress;
};
