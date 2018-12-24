// Copyright (c) 2018 The Peacenet & Alkaline Thunder

#include "CoreMinimal.h"
#include "EProgramFileOpenStatus.generated.h"

UENUM(BlueprintType)
enum class EProgramFileOpenStatus : uint8
{
	OK,
	FileNotFound,
	PermissionDenied,
	NoSuitableProgram
};
