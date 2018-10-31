// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "UDocoptValue.h"
#include "DocoptForUnrealBPLibrary.generated.h"

UCLASS()
class DOCOPTFORUNREAL_API UDocoptForUnrealBPLibrary : public UObject
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, Category = "Docopt")
		static TMap<FString, UDocoptValue*> ParseArguments(const FString& InDoc, const TArray<FString> InArgv, bool InHelp, const FString& InVersion, bool InOptionsFirst, bool& OutHasMessage, FString& OutMessage);

	static TMap<FString, UDocoptValue*> NativeParseArguments(const FString& InDoc, const TArray<FString> InArgv, bool InHelp, const FString& InVersion, bool InOptionsFirst, bool& OutHasMessage, FString& OutMessage);
};
