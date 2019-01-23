// Copyright (c) 2019 Alkaline Thunder & Bit Phoenix Software.

#pragma once

#include "CoreMinimal.h"
#include "FAdjacentNode.generated.h"

USTRUCT(BlueprintType)
struct PROJECTOGLOWIA_API FAdjacentNode
{
    GENERATED_BODY()

public:
    USTRUCT(BlueprintReadOnly, VisibleAnywhere)
    int NodeA = 0;

    USTRUCT(BlueprintReadOnly, VisibleAnywhere)
    int NodeB = 0;
};