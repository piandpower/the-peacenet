// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "EHardnessValue.generated.h"

/**
 * Represents a hardness value for a hackable service.
 */
UENUM(BlueprintType)
enum class EHardnessValue : uint8
{
	Easy,
	Normal,
	Hard,
};
