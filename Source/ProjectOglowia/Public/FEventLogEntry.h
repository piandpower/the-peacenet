// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "Text.h"
#include "FEventLogEntry.generated.h"

/**
 * Represents a parsed Event Log entry.
 */
USTRUCT(BlueprintType)
struct PROJECTOGLOWIA_API FEventLogEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Event Log Entry")
	float TimeOfDay = 0;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Event Log Entry")
	FString Username;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Event Log Entry")
	FString Message;
};
