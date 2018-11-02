// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "DocoptForUnrealBPLibrary.h"
#include "UDocoptUsageParser.h"
#include "UDocoptUtils.h"
#include "DocoptForUnreal.h"

TMap<FString, UDocoptValue*> UDocoptForUnrealBPLibrary::NativeParseArguments(const FString& InDoc, const TArray<FString> InArgv, bool InHelp, const FString& InVersion, bool InOptionsFirst, bool& OutHasMessage, FString& OutMessage)
{
	FSyntaxTree SyntaxTree;

	if(!UDocoptUsageParser::CreatePatternTree(InDoc, SyntaxTree, OutMessage))
	{
		OutHasMessage = true;
		return TMap<FString, UDocoptValue*>();
	}

	UTokens* Tokens = NewObject<UTokens>();
	Tokens->IsParsingArgv = true;
	Tokens->TokensArray = InArgv;

	PatternList ArgvPatterns = UDocoptUsageParser::ParseArgv(Tokens, SyntaxTree.Second, InOptionsFirst, OutMessage);

	if(!OutMessage.IsEmpty())
	{
		OutHasMessage = true;
		return TMap<FString, UDocoptValue*>();
	}

	UDocoptUsageParser::Extras(InHelp, !InVersion.IsEmpty(), ArgvPatterns, OutMessage);
	if(!OutMessage.IsEmpty())
	{
		if(OutMessage == TEXT("version"))
		{
			OutMessage = InVersion;
		}
		OutHasMessage = true;
		return TMap<FString, UDocoptValue*>();
	}

	TArray<ULeafPattern*> Collected;
	bool matched = SyntaxTree.First->Fix()->Match(ArgvPatterns, Collected);

	if(matched && ArgvPatterns.Num() == 0)
	{
		TMap<FString, UDocoptValue*> ret;

		for(auto p : SyntaxTree.First->Leaves())
		{
			ret.Add(p->GetName(), p->GetValue());
		}

		for(auto p : Collected)
		{
			ret.Add(p->LeafName, p->Value);
		}

		return ret;
	}

	if(matched)
	{
		FString LeftOver = UDocoptUtils::Join(InArgv, TEXT(", "));
		OutMessage = TEXT("Unexpected argument: ") + LeftOver;
		OutHasMessage = true;
		return TMap<FString, UDocoptValue*>();
	}

	OutHasMessage = true;
	OutMessage = TEXT("Arguments did not match the expected value.");
	return TMap<FString, UDocoptValue*>();
}

TMap<FString, UDocoptValue*> UDocoptForUnrealBPLibrary::ParseArguments(const FString& InDoc, const TArray<FString> InArgv, bool InHelp, const FString& InVersion, bool InOptionsFirst, bool& OutHasMessage, FString& OutMessage)
{
	return NativeParseArguments(InDoc, InArgv, InHelp, InVersion, InOptionsFirst, OutHasMessage, OutMessage);
}


