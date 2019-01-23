// Copyright (c) 2019 Alkaline Thunder & Bit Phoenix Software.

#pragma once

#include "CoreMinimal.h"
#include "ECountry.h"
#include "FEntityPosition.generated.h"

USTRUCT(BlueprintType)
struct PROJECTOGLOWIA_API FEntityPosition
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    int EntityID=0;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    FVector2D Position;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    ECountry Country;
};