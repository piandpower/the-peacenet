// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "DocoptForUnrealBPLibrary.h"
#include "DocoptForUnreal.h"

TMap<FString, UDocoptValue*> UDocoptForUnrealBPLibrary::NativeParseArguments(const FString& InDoc, const TArray<FString> InArgv, bool InHelp, const FString& InVersion, bool InOptionsFirst, bool& OutHasMessage, FString& OutMessage)
{
	return TMap<FString, UDocoptValue*>();
}

TMap<FString, UDocoptValue*> UDocoptForUnrealBPLibrary::ParseArguments(const FString& InDoc, const TArray<FString> InArgv, bool InHelp, const FString& InVersion, bool InOptionsFirst, bool& OutHasMessage, FString& OutMessage)
{
	return NativeParseArguments(InDoc, InArgv, InHelp, InVersion, InOptionsFirst, OutHasMessage, OutMessage);
}


