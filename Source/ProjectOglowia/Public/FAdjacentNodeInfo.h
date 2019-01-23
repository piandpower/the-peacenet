// Copyright (c) 2019 Alkaline Thunder & Bit Phoenix Software.

#pragma once

#include "CoreMinimal.h"
#include "ECountry.h"
#include "FAdjacentNode.h"
#include "FAdjacentNodeInfo.generated.h"

USTRUCT(BlueprintType)
struct PROJECTOGLOWIA_API FAdjacentNodeInfo
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    FString NodeName;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    ECountry Country = ECountry::Oglowia;

    UPROPERTY()
    FAdjacentNode Link;
};