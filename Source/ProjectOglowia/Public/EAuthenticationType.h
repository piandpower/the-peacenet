// Copyright (c) 2019 Alkaline Thunder & Bit Phoenix Software.

#pragma once

#include "CoreMinimal.h"
#include "EAuthenticationType.generated.h"

UENUM(BlueprintType)
enum class EAuthenticationType : uint8
{
    None,
    Basic,
    Credential,
    Crypto
};
