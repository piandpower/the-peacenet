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
#include "UWindow.h"
#include "USystemContext.h"
#include "Engine/DataAsset.h"
#include "UPeacegateProgramAsset.generated.h"

class UConsoleContext;
class UPTerminalWidget;
class APeacenetWorldStateActor;


USTRUCT(BlueprintType)
struct FAppLauncherItemInformation
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText Category;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTexture2D* Icon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool EnableMinimizeAndMaximize = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText Description;
};

/**
 * Represents a Peacegate OS graphical program.
 */
UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UPeacegateProgramAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Metadata")
	FName ExecutableName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Metadata")
	FAppLauncherItemInformation AppLauncherItem;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Program")
	TSubclassOf<UProgram> ProgramClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Metadata")
	bool IsUnlockedByDefault = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Program")
	TArray<FString> SupportedFileExtensions;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Program")
	bool IsSingleInstance = false;
};