// Copyright (c) 2019 Alkaline Thunder & Bit Phoenix Software.

#pragma once

#include "CoreMinimal.h"
#include "FFirewallRule.generated.h"

class UComputerService;

USTRUCT(BlueprintType)
struct PROJECTOGLOWIA_API FFirewallRule
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    int Port;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    bool IsFiltered = false;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    UComputerService* Service;
};