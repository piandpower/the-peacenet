// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "UHashFunctions.generated.h"

/**
 * Abstraction layer for various hash functions on strings.
 */
UCLASS(BlueprintType)
class PROJECTOGLOWIA_API UHashFunctions : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Hash Functions")
	static FString SHA256Hash(FString InString);

	UFUNCTION(BlueprintCallable, Category = "Hash Functions")
	static FString MD5Hash(FString InString);

	UFUNCTION(BlueprintCallable, Category = "Hash Functions")
	static FString CrcHash(FString InString);
};
