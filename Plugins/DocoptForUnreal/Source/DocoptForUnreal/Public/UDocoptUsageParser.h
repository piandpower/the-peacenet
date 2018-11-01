// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"

#include "UDocoptUsageParser.generated.h"

/**
 * Class for parsing Docopt usage strings.
 */
UCLASS()
class DOCOPTFORUNREAL_API UTokens : public UObject
{
	GENERATED_BODY()

public:
	TArray<FString> TokensArray;
	bool IsParsingArgv = false;
	int Index=0;

	explicit operator bool()
	{
		return Index < TokensArray.Num();
	}

	static UTokens* FromPattern(FString InSource);

	FString Current();
	FString TheRest();
	FString Pop();

};
