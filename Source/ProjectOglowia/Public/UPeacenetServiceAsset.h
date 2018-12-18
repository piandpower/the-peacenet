// Copyright (c) 2018 Alkaline Thunder & The Peacenet.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ECountry.h"
#include "UPeacenetService.h"
#include "UPeacenetServiceAsset.generated.h"

/**
 * Contains information about a known Peacenet Service.
 */
UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UPeacenetServiceAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Metadata")
    FString ServiceURL;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Service Directory")
    FText DisplayName;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Service Directory")
    FText Description;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Service Directory")
    bool IsGlobalService = false;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Service Directory")
    ECountry Country;

    UPROPERTY(EditAnywhere, Category = "User Interface")
    TSubclassOf<UPeacenetService> ServiceWidget;
};