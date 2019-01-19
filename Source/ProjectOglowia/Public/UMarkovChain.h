// Copyright (c) 2019 Alkaline Thunder & Bit Phoenix Software

#pragma once

#include "CoreMinimal.h"
#include "FMarkovSource.h"
#include "Math/RandomStream.h"
#include "CString.h"
#include "Parse.h"
#include "UMarkovChain.generated.h"

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
