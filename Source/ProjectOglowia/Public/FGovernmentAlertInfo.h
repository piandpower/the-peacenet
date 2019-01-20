// Copyright (c) 2018 Alkaline Thunderr & Bit Phoenix Software

#pragma once

#include "CoreMinimal.h"
#include "EGovernmentAlertStatus.h"
#include "FGovernmentAlertInfo.generated.h"

USTRUCT(BlueprintType)
struct FGovernmentAlertInfo
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, Category = "Government Alert")
    EGovernmentAlertStatus Status = EGovernmentAlertStatus::NoAlert;
    
    UPROPERTY(BlueprintReadOnly, Category = "Government Alert")
    float AlertLevel = 0.f;
};