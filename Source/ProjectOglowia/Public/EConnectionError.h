// Copyright (c) 2019 Alkaline Thunder & Bit Phoenix Software

#pragma once

#include "CoreMinimal.h"
#include "EConnectionError.generated.h"

UENUM(BlueprintType)
enum class EConnectionError : uint8
{
    None,
    HostNotFound,
    ConnectionRefused,
    ConnectionTimedOut
};
