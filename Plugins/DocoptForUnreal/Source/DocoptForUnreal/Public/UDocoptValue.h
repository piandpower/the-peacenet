// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "UDocoptValue.generated.h"

/**
 * Represents a kind of Docopt value.
 */
UENUM(BlueprintType)
enum class EDocoptValueKind : uint8
{
	Empty,
	Boolean,
	Number,
	String,
	List
};

/**
 * Represents a value within Docopt.
 */
UCLASS(BlueprintType)
class DOCOPTFORUNREAL_API UDocoptValue : public UObject
{
	GENERATED_BODY()

private:
	EDocoptValueKind Kind = EDocoptValueKind::Empty;
	bool BooleanValue = false;
	int NumberValue = 0;
	FString StringValue;
	TArray<FString> ListValue;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Docopt|Value")
	bool AsBoolean() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Docopt|Value")
	int AsNumber() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Docopt|Value")
	FString AsString() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Docopt|Value")
	TArray<FString> AsList() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Docopt|Value")
	bool IsValueValid() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Docopt|Value")
	bool IsBoolean() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Docopt|Value")
	bool IsNumber() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Docopt|Value")
	bool IsString() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Docopt|Value")
	bool IsList() const;

private:
	FString GetSerializedValue() const;

public:
	int HashValueData() const;

	bool ValueEquals(const UDocoptValue* InOther) const;

	void SetValue(bool InValue);
	void SetValue(int InValue);
	void SetValue(FString InValue);
	void SetValue(TArray<FString> InValue);
	void SetEmpty();
};
