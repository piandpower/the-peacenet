// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "Regex.h"
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TerminalCommandParserLibrary.generated.h"

USTRUCT(BlueprintType)
struct FPeacegateCommandInstruction 
{
	GENERATED_BODY()

	FPeacegateCommandInstruction(TArray<FString> InCommands, FString InFile, bool InOverwrite)
		: Commands(InCommands)
		, OutputFile(InFile)
		, Overwrites(InOverwrite) {}

	static FPeacegateCommandInstruction Empty()
	{
		return FPeacegateCommandInstruction(TArray<FString>(), TEXT(""), false);
	}

	FPeacegateCommandInstruction()
		: Commands(TArray<FString>())
		, OutputFile(TEXT(""))
		, Overwrites(false) {}

	UPROPERTY(BlueprintReadWrite)
		TArray<FString> Commands;

	UPROPERTY(BlueprintReadWrite)
		FString OutputFile;

	UPROPERTY(BlueprintReadWrite)
		bool Overwrites;
};


/**
 * Provides functions for tokenizing and parsing Peacegate OS commands.
 */
UCLASS()
class PROJECTOGLOWIA_API UTerminalCommandParserLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = Bash, meta = (DisplayName = "Get Command Instruction Data"))
	static FPeacegateCommandInstruction GetCommandList(const FString& InCommand, FString& OutputError);

	UFUNCTION(BlueprintCallable, Category = Bash, meta = (DisplayName = "Tokenize"))
	static TArray<FString> Tokenize(const FString& InCommand, FString& OutputError);
	
	UFUNCTION(BlueprintCallable, Category="Docopt")
	static TArray<FString> TokensFromPattern(const FString& InSource);

};
