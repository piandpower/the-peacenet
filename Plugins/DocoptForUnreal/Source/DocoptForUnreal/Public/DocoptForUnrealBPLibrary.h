// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "docopt_value.h"
#include "docopt_main.h"
#include "DocoptForUnrealBPLibrary.generated.h"

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
struct DOCOPTFORUNREAL_API FDocoptValue
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


/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/
UCLASS()
class DOCOPTFORUNREAL_API UDocoptForUnrealBPLibrary : public UObject
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, Category = "Docopt")
		static TMap<FString, FDocoptValue> ParseArguments(const FString& InDoc, const TArray<FString> InArgv, bool InHelp, const FString& InVersion, bool InOptionsFirst, bool& OutHasMessage, FString& OutMessage);

	static TMap<FString, FDocoptValue> NativeParseArguments(const FString& InDoc, const TArray<FString> InArgv, bool InHelp, const FString& InVersion, bool InOptionsFirst, bool& OutHasMessage, FString& OutMessage);

	UFUNCTION(BlueprintCallable, Category = "Docopt Value")
		static bool IsBoolean(const FDocoptValue& InValue) { return InValue.BackendValue.isBool(); }

	UFUNCTION(BlueprintCallable, Category = "Docopt Value")
		static bool IsNumber(const FDocoptValue& InValue) { return InValue.BackendValue.isLong(); }

	UFUNCTION(BlueprintCallable, Category = "Docopt Value")
		static bool IsString(const FDocoptValue& InValue) { return InValue.BackendValue.isString(); }

	UFUNCTION(BlueprintCallable, Category = "Docopt Value")
		static bool IsArrayOfStrings(const FDocoptValue& InValue) { return InValue.BackendValue.isStringList(); }

	UFUNCTION(BlueprintCallable, Category = "Docopt Value")
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

	UFUNCTION(BlueprintCallable, Category = "Docopt Value")
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
