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
#include "UPeacegateFileSystem.h"
#include "SlateFontInfo.h"
#include "ETerminalColor.h"
#include "EFileOpenResult.h"
#include "Camera/CameraComponent.h"
#include "CommonUtils.generated.h"

class UPeacenetSaveGame;
class USystemContext;

/**
 * Common utilities used throughout the entire game.
 */
UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UCommonUtils : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Peacegate|Setup")
	static void ParseCharacterName(const FString InCharacterName, FString& OutUsername, FString& OutHostname);

private:
	template<typename T>
	static int Partition(TArray<T>& InArray, int Start, int End, TFunction<bool(const T& InA, const T& InB)> InComparer);

	template<typename T>
	static void QuickSortInternal(TArray<T>& InArray, int Start, int End, TFunction<bool(const T& InA, const T& InB)> InComparer);
public:
	
	template<typename T>
	static TArray<T> QuickSort(TArray<T> InArray, TFunction<bool(const T& InA, const T& InB)> InComparer);

	template<typename T>
	static bool BinarySearch(TArray<T> InArray, TFunction<int(const T& InA)> InComparer, T& OutElement);

	UFUNCTION(BlueprintCallable, Category = "Peacegate", BlueprintPure)
	static FText GetFriendlyFilesystemStatusCode(const EFilesystemStatusCode InStatusCode);

	UFUNCTION(BlueprintCallable, Category = "Peacegate")
	static UPeacegateFileSystem* CreateFilesystem(USystemContext* InSystemContext, int InUserID);

	UFUNCTION(BlueprintCallable, Category = "Terminal")
	static FLinearColor GetTerminalColor(ETerminalColor InColor);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Terminal")
	static FString GetTerminalColorCode(ETerminalColor InColor);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Terminal")
	static bool IsColorCode(FString InControlCode, ETerminalColor& OutColor);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Trigonometry Bullshit")
	static float GetRotation(FVector2D InA, FVector2D InB);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "URL parsing")
	static void ParseURL(FString InURL, FString& OutUsername, FString& OutHost, int& OutPort, FString& OutPath, bool& HasPath, bool& HasUser, bool& HasPort);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Common")
	static void GetFriendlyFileOpenText(EFileOpenResult InResult, FString& OutTitle, FString& OutDescription);

	static void MeasureChar(const TCHAR InChar, const FSlateFontInfo& InSlateFont, float& OutWidth, float& OutHeight);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Clipboard")
	static bool GetClipboardText(FString& OutText);

	UFUNCTION(BlueprintCallable, Category = "Clipboard")
	static void PutClipboardText(FString InText);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	static void SetEnableBloom(UCameraComponent* InCamera, bool InEnableBloom);
};

template<typename T>
inline int UCommonUtils::Partition(TArray<T>& InArray, int Start, int End, TFunction<bool(const T&InA, const T&InB)> InComparer)
{
	T& pivot = InArray[End];
	int i = Start;
	for (int j = Start; j < End - 1; j++)
	{
		if (InComparer(InArray[j], pivot))
		{
			if (i != j)
			{
				T& temp = InArray[i];
				InArray[i] = InArray[j];
				InArray[j] = temp;
			}
			i++;
		}
	}
	T& iTemp = InArray[i];
	InArray[i] = InArray[End];
	InArray[End] = iTemp;
	return i;
}

template<typename T>
inline void UCommonUtils::QuickSortInternal(TArray<T>& InArray, int Start, int End, TFunction<bool(const T&InA, const T&InB)> InComparer)
{
	if (Start < End)
	{
		int p = Partition(InArray, Start, End, InComparer);
		QuickSortInternal(InArray, Start, p - 1, InComparer);
		QuickSortInternal(InArray, p + 1, End, InComparer);
	}
}

template<typename T>
inline TArray<T> UCommonUtils::QuickSort(TArray<T> InArray, TFunction<bool(const T&InA, const T&InB)> InComparer)
{
	TArray<T> Sort = InArray;
	QuickSortInternal(Sort, 0, Sort.Num() - 1, InComparer);
	return Sort;
}

template<typename T>
inline bool UCommonUtils::BinarySearch(TArray<T> InArray, TFunction<int(const T&InA)> InComparer, T & OutElement)
{
	if (!InArray.Num())
		return false;

	// NOTE: THIS ALGORITHM ONLY WORKS IF THE ARRAY IS SORTED. Don't be a fucking drunky. Use a QuickSort.

	int min = 0;
	int max = InArray.Num();

	while (max - min != 0)
	{
		int average = (min + max) / 2;

		int guessResult = InComparer(InArray[average]);

		if (guessResult == 0)
		{
			OutElement = InArray[average];
			return true;
		}
		else if (guessResult > 0)
		{
			min = average + 1;
		}
		else
		{
			max = average - 1;
		}
	}

	return false;
}
