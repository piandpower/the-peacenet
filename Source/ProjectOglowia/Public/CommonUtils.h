// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "UPeacegateFileSystem.h"
#include "SlateFontInfo.h"
#include "ETerminalColor.h"
#include "CommonUtils.generated.h"

class USystemContext;

/**
 * Common utilities used throughout the entire game.
 */
UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UCommonUtils : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Peacegate", BlueprintPure)
	static FText GetFriendlyFilesystemStatusCode(const EFilesystemStatusCode InStatusCode);

	UFUNCTION(BlueprintCallable, Category = "Peacegate")
	static UPeacegateFileSystem* CreateFilesystem(USystemContext* InSystemContext, int InUserID);

	UFUNCTION(BlueprintCallable, Category = "Terminal")
	FLinearColor GetTerminalColor(ETerminalColor InColor);
	
	static void MeasureChar(const TCHAR InChar, const FSlateFontInfo& InSlateFont, float& OutWidth, float& OutHeight);
};
