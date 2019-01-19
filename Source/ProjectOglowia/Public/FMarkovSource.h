// Copyright (c) 2018 Alkaline Thunder & Bit Phoenix Software

#pragma once

#include "CoreMinimal.h"
#include "FMarkovSource.generated.h"

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