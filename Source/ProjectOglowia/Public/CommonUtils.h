// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "UPeacegateFileSystem.h"
#include "FEventLogEntry.h"
#include "SlateFontInfo.h"
#include "ETerminalColor.h"
#include "Camera/CameraComponent.h"
#include "FEmailMessage.h"
#include "FEmailAttachment.h"
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
	UFUNCTION(BlueprintCallable, Category = "Event Log Entry", BlueprintPure)
	static FString ParseEventLogEntryToString(const FEventLogEntry& InEventLogEntry);

	UFUNCTION()
	static FEventLogEntry ReadEventLogEntry(FString InString);

	UFUNCTION()
	static TArray<FEventLogEntry> ReadEventLogFile(FString InString);

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

	static void MeasureChar(const TCHAR InChar, const FSlateFontInfo& InSlateFont, float& OutWidth, float& OutHeight);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	static void SetEnableBloom(UCameraComponent* InCamera, bool InEnableBloom);

	UFUNCTION()
	void SendEmailChecked(UPeacenetSaveGame* InSaveGame, int FromEntity, int ToEntity, const FText& Subject, const FText& Message, TArray<FEmailAttachment> InAttachments = TArray<FEmailAttachment>(), TArray<FEmailMission> InMissions = TArray<FEmailMission>());
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
