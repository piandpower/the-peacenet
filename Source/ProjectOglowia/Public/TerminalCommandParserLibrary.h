/********************************************************************************
 * The Peacenet - bit::phoenix("software");
 * 
 * MIT License
 *
 * Copyright (c) 2018-2019 Michael VanOverbeek, Declan Hoare, Ian Clary, 
 * Trey Smith, Richard Moch, Victor Tran and Warren Harris
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * Contributors:
 *  - Michael VanOverbeek <alkaline@bitphoenixsoftware.com>
 *
 ********************************************************************************/


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
	static FPeacegateCommandInstruction GetCommandList(const FString& InCommand, FString InHome, FString& OutputError);

	UFUNCTION(BlueprintCallable, Category = Bash, meta = (DisplayName = "Tokenize"))
	static TArray<FString> Tokenize(const FString& InCommand, const FString& Home, FString& OutputError);
	
};
