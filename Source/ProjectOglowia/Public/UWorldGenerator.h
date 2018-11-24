// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "Math/RandomStream.h"
#include "FPeacenetIdentity.h"
#include "FComputer.h"
#include "ECountry.h"
#include "AsyncWork.h"
#include "CString.h"
#include "Parse.h"
#include "UMarkovTrainingDataAsset.h"
#include "UWorldGenerator.generated.h"

class USystemContext;
class UComputerService;
class UHackableAsset;
class UComputerTypeAsset;
class UCompanyTypeAsset;
class APeacenetWorldStateActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWorldGenCompleteEvent);

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
};

UCLASS(BlueprintType)
class PROJECTOGLOWIA_API UWorldGeneratorStatus : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "World Generator")
	FText Status;

	UPROPERTY(BlueprintReadOnly, Category = "World Generator")
	float Percentage = 0.f;

	UPROPERTY(BlueprintAssignable, Category = "World Generator")
	FWorldGenCompleteEvent WorldGenerationCompleted;
};

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


public:
	static TArray<FString> FilterTrainingData(TArray<UMarkovTrainingDataAsset*> InAssets, EMarkovTrainingDataUsage Usage);

	UFUNCTION()
	static FString MakeName(FString InWord);

	UFUNCTION(BlueprintCallable, Category = "World Generation")
	static int32 GetSeedFromString(const FString& InSeedString);

	UFUNCTION(BlueprintCallable, Category = "World Generation")
	static FString GenerateRandomPassword(const FRandomStream& InGenerator, int InLength);

	UFUNCTION(BlueprintCallable, Category = "World Generation")
	static FRandomStream GetRandomNumberGenerator(int32 InSeed);

	UFUNCTION(BlueprintCallable, Category = "World Generation")
	static FString GenerateRandomName(const FRandomStream& InGenerator, const TArray<FString> InFirstNames, TArray<FString> InLastNames);

	UFUNCTION(BlueprintCallable, Category = "World Generation")
	static UWorldGeneratorStatus* GenerateCharacters(const APeacenetWorldStateActor* InWorld, const FRandomStream& InRandomStream, UPeacenetSaveGame* InSaveGame);

	static FString GenerateIPAddress(const FRandomStream& InRandomStream, const ECountry InCountry, int InEntityID);
	static FString GenerateIPAddress(const FRandomStream& InRandomStream, const ECountry InCountry, const FComputer& InComputer);

	UFUNCTION()
	static void GenerateSystemDirectories(USystemContext* InSystemContext);

	UFUNCTION()
	static UMarkovChain* CreateMarkovChain(const TArray<FString> InTrainingData, FRandomStream InRandomStream);

	UFUNCTION()
	static FString GenerateWordString(UMarkovChain* InMarkovChain, int Length);

	UFUNCTION()
	static void CreateFilesystem(FPeacenetIdentity& InCharacter, FComputer& InComputer, const FRandomStream& InGenerator, FString InHostname = "");
};

class PROJECTOGLOWIA_API FWorldGenTask : public FNonAbandonableTask
{
	friend class FAutoDeleteAsyncTask<FWorldGenTask>;

public:
	FWorldGenTask(UPeacenetSaveGame* InSaveGame, const FRandomStream InRandomStream, UWorldGeneratorStatus* InStatus, TArray<UMarkovTrainingDataAsset*> InTrainingData, TArray<UComputerTypeAsset*> InComputerTypes, UComputerTypeAsset* InPersonalComputerType, TArray<UCompanyTypeAsset*> InCompanyTypes, TArray<UComputerService*> InComputerServices)
		: SaveGame(InSaveGame)
		, RandomStream(InRandomStream)
		, Status(InStatus)
		, TrainingData(InTrainingData)
		, ComputerTypes(InComputerTypes)
		, PersonalComputerType(InPersonalComputerType)
		, CompanyTypes(InCompanyTypes)
		, ComputerServices(InComputerServices) {}

	UPeacenetSaveGame* SaveGame;
	FRandomStream RandomStream;
	UWorldGeneratorStatus* Status;
	TArray<UMarkovTrainingDataAsset*> TrainingData;
	TArray<UComputerTypeAsset*> ComputerTypes;
	TArray<UCompanyTypeAsset*> CompanyTypes;
	UComputerTypeAsset* PersonalComputerType;
	TArray<UComputerService*> ComputerServices;

	TArray<UComputerService*> GetSupportedServices(UHackableAsset* InHackable);
	void FilterSkillLevel(TArray<UComputerService*>& InServices, int InSkill);
	bool PortIsTaken(int InPort, FComputer& InComputer);

	void DoWork();

	FORCEINLINE TStatId GetStatId() const 
	{ 
		RETURN_QUICK_DECLARE_CYCLE_STAT(FWorldGenTask, STATGROUP_ThreadPoolAsyncTasks); 
	}
};