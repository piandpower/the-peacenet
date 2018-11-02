// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "UPattern.h"
#include "UDocoptUsageParser.generated.h"

using FSyntaxTree = FPair<URequired*, TArray<UOption*>>;

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

UCLASS()
class DOCOPTFORUNREAL_API UDocoptUsageParser : public UObject
{
	GENERATED_BODY()

public:
	template<typename T>
	static TArray<T*> FlatFilter(UPattern* InPattern);

	static TArray<FString> ParseSection(FString InName, FString InSource);
	static bool IsArgumentSpec(FString InToken);
	static TArray<FString> LongOptions(TArray<UOption*> InList);
	static PatternList ParseLong(UTokens* InTokens, TArray<UOption*>& OutOptions, FString& OutError);
	static PatternList ParseShort(UTokens* InTokens, TArray<UOption*>& OutOptions, FString& OutError);
	static PatternList ParseExpression(UTokens* InTokens, TArray<UOption*>& OutOptions, FString& OutError);
	static PatternList ParseAtom(UTokens* InTokens, TArray<UOption*>& OutOptions, FString& OutError);
	static PatternList ParseSequence(UTokens* InTokens, TArray<UOption*>& OutOptions, FString& OutError);
	static UPattern* TryCollapseToRequired(PatternList& InPatterns);
	static UPattern* TryCollapseToEither(PatternList& InPatterns);
	static URequired* ParsePattern(FString InSource, TArray<UOption*>& OutOptions, FString& OutError);
	static FString FormatUsage(FString InSection);
	static PatternList ParseArgv(UTokens* InTokens, TArray<UOption*>& OutOptions, bool InOptionsFirst, FString& OutError);
	static TArray<UOption*> ParseDefaults(FString InDoc);
	static bool IsOptionSet(PatternList& InOptions, FString InOpt1, FString InOpt2 = TEXT(""));
	static bool Extras(bool InHelp, bool InVersion, PatternList& InOptions, FString& OutMessage);
	static bool CreatePatternTree(FString InDoc, FSyntaxTree& OutSyntaxTree, FString& OutError);
};