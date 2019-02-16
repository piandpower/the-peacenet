// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "docopt_value.h"
#include "DocoptForUnrealBPLibrary.generated.h"

UCLASS(BlueprintType)
class DOCOPTFORUNREAL_API UDocoptValue : public UObject
{
	GENERATED_BODY()

public:
	docopt::value UnderlyingValue;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Docopt|Value")
	bool IsBoolean();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Docopt|Value")
	bool IsNumber();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Docopt|Value")
	bool IsString();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Docopt|Value")
	bool IsList();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Docopt|Value")
		bool AsBoolean();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Docopt|Value")
		int AsNumber();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Docopt|Value")
		FString AsString();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Docopt|Value")
		TArray<FString> AsList();
};

UCLASS()
class DOCOPTFORUNREAL_API UDocoptForUnrealBPLibrary : public UObject
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, Category = "Docopt")
		static TMap<FString, UDocoptValue*> ParseArguments(const FString& InDoc, const TArray<FString> InArgv, bool InHelp, const FString& InVersion, bool InOptionsFirst, bool& OutHasMessage, FString& OutMessage);

	static TMap<FString, UDocoptValue*> NativeParseArguments(const FString& InDoc, const TArray<FString> InArgv, bool InHelp, const FString& InVersion, bool InOptionsFirst, bool& OutHasMessage, FString& OutMessage);
};
