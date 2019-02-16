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

#include "CoreMinimal.h"
#include "TerminalCommand.h"
#include "Engine/DataAsset.h"
#include "CommandInfo.generated.h"

USTRUCT(BlueprintType)
struct PROJECTOGLOWIA_API FOptionDescription 
{
	GENERATED_BODY()

	/** The options associated with this description. Each option in this array is treated as a synonym. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> Options;

	/** The description of the options when shown to the player in-game using the man command.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;
};

USTRUCT(BlueprintType)
struct PROJECTOGLOWIA_API FCommandInfoS
{
	GENERATED_BODY()

		// The name of the command that the player will need to type in. Also displayed in help listings and used to identify the command when interacting with Peacegate OS's API.
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName CommandName;

	// The description of the command, shown in the help listing and manpage.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString LongDescription;

	// The usage syntax strings for the command. This is used internally while parsing command-line arguments. Each element in the array represents a different line in the resulting Docopt usage string. You do NOT need to include the "Usage: " delimeter as Peacenet will do this for you when the asset loads.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> UsageStrings;

	// A list of option descriptions for the command, displayed in the "Options" section of the manpage, and used by Docopt to determine option synonyms during parsing.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FOptionDescription> OptionDescriptions;

	// A C++ or Blueprint class inheriting from UTerminalCommand which holds the state of and handles running this command.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ATerminalCommand> CommandClass;
};

UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UCommandInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FCommandInfoS Info;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool UnlockedByDefault = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool ShowInHelp = true;

	UFUNCTION(BlueprintCallable, Category="Peacenet Utility Functions")
	static FString BuildManualPage(const FCommandInfoS& InCommandInfo);

	UFUNCTION(BlueprintCallable, Category = "Peacenet Utility Functions")
	static FString BuildDisplayUsageString(const FCommandInfoS& InCommandInfo);

	UFUNCTION(BlueprintCallable, Category = "Peacenet Utility Functions")
	static FString BuildInternalUsageString(const FCommandInfoS& InCommandInfo);
};