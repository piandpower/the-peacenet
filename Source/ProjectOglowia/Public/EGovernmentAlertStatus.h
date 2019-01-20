// Copyright (c) 2018 Alkaline Thunder & Bit Phoenix Software

#pragma once

#include "CoreMinimal.h"
#include "EGovernmentAlertStatus.generated.h"

UENUM(BlueprintType)
enum class EGovernmentAlertStatus : uint8
{
    NoAlert,
    BOLO,
    Active
};