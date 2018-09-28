// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "Math/RandomStream.h"
#include "FPeacenetIdentity.h"
#include "FComputer.h"
#include "ECountry.h"
#include "CString.h"
#include "Parse.h"
#include "UWorldGenerator.generated.h"

/**
 * Encapsulates methods used for generating a Peacenet world.
 */
UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UWorldGenerator : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "World Generation")
	static int32 GetSeedFromString(const FString& InSeedString);

	UFUNCTION(BlueprintCallable, Category = "World Generation")
	static FString GenerateRandomPassword(const FRandomStream& InGenerator, int InLength);

	UFUNCTION(BlueprintCallable, Category = "World Generation")
	static FRandomStream GetRandomNumberGenerator(int32 InSeed);

	UFUNCTION(BlueprintCallable, Category = "World Generation")
	static FString GenerateRandomName(const FRandomStream& InGenerator, const TArray<FString> InFirstNames, TArray<FString> InLastNames);

	UFUNCTION(BlueprintCallable, Category = "World Generation")
	static void GenerateCharacters(UPARAM(Ref) TArray<FPeacenetIdentity> CharacterArray, UPARAM(Ref) TArray<FComputer> ComputerArray, const FRandomStream& InGenerator, int32 InCharacterCount, const TArray<FString> InFirstNames, TArray<FString> InLastNames);
};
