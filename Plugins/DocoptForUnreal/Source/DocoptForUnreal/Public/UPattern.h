// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "UPattern.generated.h"


class UPattern; // A little redundant?
class ULeafPattern;
class UDocoptValue;

using PatternList = TArray<UPattern*>;
using UniquePatternSet = TArray<UPattern*>;

template<typename TFirst, typename TSecond>
struct FPair
{
	TFirst First;
	TSecond Second;

	FPair(TFirst InFirst, TSecond InSecond)
		: First(InFirst)
		, Second(InSecond) {};

	FPair() {};
};

/**
 * Represents a Docopt pattern.
 */
UCLASS(BlueprintType)
class DOCOPTFORUNREAL_API UPattern : public UObject
{
	GENERATED_BODY()

protected:
	const FString EmptyName = TEXT("");

public:
	virtual PatternList Flatten(bool(*filter)(UPattern const*)) { return PatternList(); }
	virtual void CollectLeaves(TArray<ULeafPattern*>& OutLeaves) {}
	TArray<ULeafPattern*> Leaves();
	virtual bool Match(PatternList& InLeft, TArray<ULeafPattern*>& Collected) const { return false; }
	virtual FString const& Name() const { return EmptyName; }
	virtual bool HasValue() const { return false; }
	virtual int HashPattern() const { return 0; }
};

UCLASS(BlueprintType)
class DOCOPTFORUNREAL_API ULeafPattern : public UPattern
{
	GENERATED_BODY()

public:
	virtual PatternList Flatten(bool(*filter)(UPattern const*)) override;
	virtual void CollectLeaves(TArray<ULeafPattern*>& OutLeaves) override;
	virtual bool Match(PatternList& InLeft, TArray<ULeafPattern*>& Collected) const override;
	virtual bool HasValue() const override;

	UDocoptValue* GetValue() const;
	void SetValue(UDocoptValue* InValue);

	virtual FString const& Name() const override;

	virtual int HashPattern() const override;

protected:
	virtual FPair<int, ULeafPattern*> SingleMatch(PatternList const&) const { return FPair<int, ULeafPattern*>(); }

public:
	UPROPERTY()
	FString LeafName;

	UPROPERTY()
	UDocoptValue* Value;

	void SetValue(bool InValue);
	void SetValue(int InValue);
	void SetValue(FString InValue);
	void SetValue(TArray<FString> InValue);

};

UCLASS(BlueprintType)
class DOCOPTFORUNREAL_API UBranchPattern : public UPattern
{
	GENERATED_BODY()

public:
	UPattern* Fix();
	virtual UDocoptValue* const GetValue() const;
	virtual PatternList Flatten(bool(*filter)(UPattern const*)) override;
	virtual void CollectLeaves(TArray<ULeafPattern*>& OutLeaves) override;
	void SetChildren(PatternList InChildren);
	PatternList const& GetChildren() const;
	virtual void FixIdentities(UniquePatternSet& InPatterns);
	virtual int HashPattern() const override;
	virtual FString const& Name() const override;

private:
	void FixRepeatingArguments();

public:
	PatternList Children;
};

UCLASS(BlueprintType)
class DOCOPTFORUNREAL_API UArgument : public ULeafPattern
{
	GENERATED_BODY()

protected:
	virtual FPair<int, ULeafPattern*> SingleMatch(PatternList const&) const override;
};

UCLASS(BlueprintType)
class DOCOPTFORUNREAL_API UCommand : public UArgument
{
	GENERATED_BODY()

protected:
	virtual FPair<int, ULeafPattern*> SingleMatch(PatternList const&) const override;
};

UCLASS(BlueprintType)
class DOCOPTFORUNREAL_API UOption : public ULeafPattern
{
	GENERATED_BODY()

public:
	static UOption* Parse(FString const& InOptionDescription);
	FString ShortOption;
	FString LongOption;
	int ArgumentCount = 0;
	virtual int HashPattern() const override;


protected:
	virtual FPair<int, ULeafPattern*> SingleMatch(PatternList const&) const override;
};

UCLASS(BlueprintType)
class DOCOPTFORUNREAL_API URequired : public UBranchPattern
{
	GENERATED_BODY()

public:
	virtual bool Match(PatternList& InLeft, TArray<ULeafPattern*>& Collected) const override;
};

UCLASS(BlueprintType)
class DOCOPTFORUNREAL_API UOptional : public UBranchPattern
{
	GENERATED_BODY()

public:
	virtual bool Match(PatternList& InLeft, TArray<ULeafPattern*>& Collected) const override;
};

UCLASS(BlueprintType)
class DOCOPTFORUNREAL_API UOptionsShortcut : public UOptional
{
	GENERATED_BODY()
};

UCLASS(BlueprintType)
class DOCOPTFORUNREAL_API UOneOrMore : public UBranchPattern
{
	GENERATED_BODY()

public:
	virtual bool Match(PatternList& InLeft, TArray<ULeafPattern*>& Collected) const override;
};

UCLASS(BlueprintType)
class DOCOPTFORUNREAL_API UEither : public UBranchPattern
{
	GENERATED_BODY()

public:
	virtual bool Match(PatternList& InLeft, TArray<ULeafPattern*>& Collected) const override;
};