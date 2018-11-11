// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "Text.h"
#include "UEmailParser.generated.h"

/**
 * Represents a Peacenet email.
 */
USTRUCT(BlueprintType)
struct PROJECTOGLOWIA_API FEmail
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FText From;

	UPROPERTY(BlueprintReadOnly)
	FText To;

	UPROPERTY(BlueprintReadOnly)
	FText Subject;

	UPROPERTY(BlueprintReadOnly)
	FText Content;
};

/**
 * Parses and serializes emails.
 */
UCLASS()
class PROJECTOGLOWIA_API UEmailParser : public UObject
{
	GENERATED_BODY()

public:
	static FEmail DeserializeEmail(const FString InJson);
	static FString SerializeEmail(const FEmail InEmail);
};
