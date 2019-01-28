// Copyright (c) 2018 The Peacenet & Alkaline Thunder

// that feeling when you go a FUCKING MONTH without adding a #pragma once
#pragma once

#include "CoreMinimal.h"
#include "EFileOpenResult.generated.h"

UENUM(BlueprintType)
enum class EFileOpenResult : uint8
{
	OK,
	FileNotFound,
	PermissionDenied,
	NoSuitableProgram
};
