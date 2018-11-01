// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "Regex.h"
#include "UDocoptUtils.generated.h"

USTRUCT()
struct FStringPartition
{
	GENERATED_BODY()

public:
	FString First;
	FString Second;
	FString Third;
};

/**
 * Utilities used by the Docopt library.
 */
UCLASS()
class DOCOPTFORUNREAL_API UDocoptUtils : public UObject
{
	GENERATED_BODY()

public:
	static bool StartsWith(FString const& InStr, FString const& InPrefix);
	static FString Trim(FString InStr);
	static TArray<FString> Split(FString InStr, int InPosition = 0);
	static FStringPartition Partition(FString InStr, FString const& InPoint);
	static FString Join(TArray<FString> InArray, FString InDelimeter);
	static TArray<FString> RegexSplit(FString InText, FRegexMatcher InRegex);
	static int FindFirstNotOf(FString InStr, FString InChars, int InStart);
	static int FindFirstOf(FString InStr, FString InChars, int InStart);
	static FString SubString(FString InStr, int InStart, int InEnd);
	static void Resize(FString& InStr, int NewSize);
};
