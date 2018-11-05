// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UMarkovTrainingDataAsset.generated.h"


UENUM(BlueprintType)
enum class EMarkovTrainingDataUsage : uint8
{
	MaleFirstNames,
	FemaleFirstNames,
	LastNames,
	Hostnames,
	FileNames
};

/**
 * A data asset that can be used as training data for markov chains used by the Peacenet World generator.
 */
UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UMarkovTrainingDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EMarkovTrainingDataUsage Usage = EMarkovTrainingDataUsage::MaleFirstNames;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FString> TrainingData;

};
