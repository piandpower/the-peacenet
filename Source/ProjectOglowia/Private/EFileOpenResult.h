// Copyright (c) 2018 The Peacenet & Alkaline Thunder

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
