// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "docopt_main.h"
#include "docopt_value.h"
#include "DocoptBlueprintAPI.generated.h"

UENUM(Blueprintable)
enum class EDocoptKind : uint8
{
	Empty,
	Boolean,
	String,
	Number,
	StringList
};

USTRUCT(BlueprintType)
struct DOCOPT_API FDocoptValue
{
	GENERATED_BODY()

public:
	FDocoptValue() : BackendValue(docopt::value()) {}
	FDocoptValue(docopt::value backVal) : BackendValue(backVal) {}

	EDocoptKind GetKind()
	{
		if (BackendValue.isBool())
			return EDocoptKind::Boolean;
		if (BackendValue.isLong())
			return EDocoptKind::Number;
		if (BackendValue.isString())
			return EDocoptKind::String;
		if (BackendValue.isStringList())
			return EDocoptKind::StringList;

		return EDocoptKind::Empty;
	}

	docopt::value BackendValue;
};

/**
 * Blueprint API that allows the use of the docopt command-line interface language in Unreal Engine.
 */
UCLASS()
class DOCOPT_API UDocoptBlueprintAPI : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Docopt")
	static TMap<FString, FDocoptValue> ParseArguments(const FString& InDoc, const TArray<FString> InArgv, bool InHelp, const FString& InVersion, bool InOptionsFirst, bool& OutHasMessage, FString& OutMessage);
	
	UFUNCTION(BlueprintCallable, Category="Docopt Value")
		static bool IsBoolean(const FDocoptValue& InValue) { return InValue.BackendValue.isBool(); }
	
	UFUNCTION(BlueprintCallable, Category = "Docopt Value")
	static bool IsNumber(const FDocoptValue& InValue) { return InValue.BackendValue.isLong(); }

	UFUNCTION(BlueprintCallable, Category = "Docopt Value")
	static bool IsString(const FDocoptValue& InValue) { return InValue.BackendValue.isString(); }

	UFUNCTION(BlueprintCallable, Category = "Docopt Value")
	static bool IsArrayOfStrings(const FDocoptValue& InValue) { return InValue.BackendValue.isStringList(); }

	UFUNCTION(BlueprintCallable, Category="Docopt Value")
	static bool DoesKindMatch(UPARAM(Ref) FDocoptValue& InValue, EDocoptKind InKind) { return InValue.GetKind() == InKind; }
	
	UFUNCTION(BlueprintCallable, Category = "Docopt Value")
	static int32 ValueAsNumber(const FDocoptValue& InValue)
	{
		if (IsNumber(InValue))
			return InValue.BackendValue.asLong();
		return 0;
	}

	UFUNCTION(BlueprintCallable, Category = "Docopt Value")
	static bool ValueAsBoolean(const FDocoptValue& InValue)
	{
		if (IsBoolean(InValue))
			return InValue.BackendValue.asBool();
		return false;
	}

	UFUNCTION(BlueprintCallable, Category = "Docopt Value")
	static FString ValueAsString(const FDocoptValue& InValue)
	{
		if (IsString(InValue))
			return FString(InValue.BackendValue.asString().c_str());
		return TEXT("");
	}

	UFUNCTION(BlueprintCallable, Category="Docopt Value")
	static FString KindAsString(UPARAM(Ref) FDocoptValue& InValue)
	{
		auto kind = InValue.GetKind();

		switch (kind)
		{
		case EDocoptKind::Boolean:
			return TEXT("Boolean");
		case EDocoptKind::Number:
			return TEXT("Number");
		case EDocoptKind::String:
			return TEXT("String");
		case EDocoptKind::StringList:
			return TEXT("StringArray");
		default:
			return TEXT("Empty");
		}
	}

	UFUNCTION(BlueprintCallable, Category = "Docopt Value")
	static TArray<FString> ValueAsArrayOfStrings(const FDocoptValue& InValue)
	{
		if (IsArrayOfStrings(InValue)) 
		{
			TArray<FString> arr;
			std::vector<std::string> orig = InValue.BackendValue.asStringList();
			
			for (auto& val : orig) {
				arr.Add(FString(val.c_str()));
			}

			return arr;
		}
		return TArray<FString>();
	}




};
