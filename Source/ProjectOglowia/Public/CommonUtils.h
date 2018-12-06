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
