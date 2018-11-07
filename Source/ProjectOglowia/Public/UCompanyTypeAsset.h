// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/Texture2D.h"
#include "UComputerTypeAsset.h"
#include "UCompanyTypeAsset.generated.h"

/**
 * Represents a type of company/faction within Peacenet.
 */
UCLASS(Blueprintable, BlueprintType)
class PROJECTOGLOWIA_API UCompanyTypeAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Peacenet Asset")
	FName InternalID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Company Type")
	FText Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Company Type")
	FText Description;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Company Type")
	UTexture2D* Icon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Network")
	TArray<UComputerTypeAsset*> ComputerTypes;

};
