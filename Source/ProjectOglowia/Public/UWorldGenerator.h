// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "Math/RandomStream.h"
#include "FPeacenetIdentity.h"
#include "FComputer.h"
#include "ECountry.h"
#include "CString.h"
#include "Parse.h"
#include "UMarkovTrainingDataAsset.h"
#include "UWorldGenerator.generated.h"

class USystemContext;

USTRUCT()
struct PROJECTOGLOWIA_API FMarkovSource
{
	GENERATED_BODY()

private:
	FString Data;
	TArray<TCHAR> Chars;

public:
	void SetCount(int InCount);
	void Rotate(TCHAR NextChar);
	bool IsLessThan(const FMarkovSource& OtherSource);
	bool IsStartSource();

	bool operator==(const FMarkovSource& Other) const;

	TArray<TCHAR> const& GetChars() const;

	FString ToString() const;
};

// hash function for FMarkovSource so we can use it as a keytype in a TMap.
FORCEINLINE uint32 GetTypeHash(const FMarkovSource& Source)
{
	return FCrc::MemCrc32(Source.GetChars().GetData(), Source.GetChars().Num() * sizeof(TCHAR));
}

UCLASS()
class PROJECTOGLOWIA_API UMarkovChain : public UObject
{
	GENERATED_BODY()

private:
	TMap<FMarkovSource, TMap<TCHAR, int>> MarkovMap;

	TCHAR GetNext(FMarkovSource InSource);

	FMarkovSource RandomSource();

	bool IsDeadEnd(FMarkovSource InSource, int Depth);

	TCHAR GetNextCharGuarantee(FMarkovSource InSource, int InSteps);

	FRandomStream Random;

	int SourceCount = 0;

public:
	void Init(TArray<FString> InArray, int N, FRandomStream InRng);

	FString GetMarkovString(int InLength);
};

/**
 * Encapsulates methods used for generating a Peacenet world.
 */
UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UWorldGenerator : public UObject
{
	GENERATED_BODY()

private:
	static TArray<FString> FilterTrainingData(TArray<UMarkovTrainingDataAsset*> InAssets, EMarkovTrainingDataUsage Usage);

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
		static void GenerateCharacters(const FRandomStream& InRandomStream, UPeacenetSaveGame* InSaveGame);

	UFUNCTION()
	static void GenerateSystemDirectories(USystemContext* InSystemContext);

	UFUNCTION()
	static UMarkovChain* CreateMarkovChain(const TArray<FString> InTrainingData, FRandomStream InRandomStream);

	UFUNCTION()
	static FString GenerateWordString(UMarkovChain* InMarkovChain, int Length);

	UFUNCTION()
	static void CreateFilesystem(FComputer& InComputer, const FRandomStream& InGenerator);
};
