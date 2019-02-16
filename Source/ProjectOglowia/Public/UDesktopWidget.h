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
#include "UserWidget.h"
#include "UPeacegateProgramAsset.h" 
#include "EGovernmentAlertStatus.h"
#include "FGovernmentAlertInfo.h"
#include "UPeacegateFileSystem.h"
#include "FGameRules.h"
#include "UDesktopWidget.generated.h"

class USystemContext;
class UConsoleContext;
class UPTerminalWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActiveProgramCloseEvent);

USTRUCT()
struct PROJECTOGLOWIA_API FDesktopNotification
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FText Title;

	UPROPERTY()
	FText Message;

	UPROPERTY()
	UTexture2D* Icon;
};

/**
 * Base class of a Desktop environment.
 */
UCLASS(Blueprintable, Abstract)
class PROJECTOGLOWIA_API UDesktopWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "System")
	USystemContext* GetSystemContext();

	UPROPERTY()
	FGovernmentAlertInfo AlertInfo;

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Government Alert")
	EGovernmentAlertStatus GetAlertStatus();

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "System")
	void ExecuteCommand(const FString& InCommand);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "System")
	void SwitchWorkspace(int InWorkspaceNumber);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "System")
	void MoveActiveProgramToWorkspace(int InWorkspaceNumber);

	UFUNCTION(BlueprintCallable, Category = "System")
	void CloseActiveProgram();

	FActiveProgramCloseEvent EventActiveProgramClose;

public:
	UFUNCTION(BlueprintCallable, Category = "Desktop")
	UProgram* SpawnProgramFromClass(TSubclassOf<UProgram> InClass, const FText& InTitle, UTexture2D* InIcon);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Desktop")
	FPeacenetIdentity MyCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Desktop")
	FComputer MyComputer;

private:
	bool bIsWaitingForNotification = false;

protected:
	UFUNCTION(BlueprintCallable, Category = "Console")
	UConsoleContext* CreateConsole(UPTerminalWidget* InTerminal);

public:
	UPROPERTY()
	USystemContext * SystemContext;

	UFUNCTION()
	void SetWallpaper(UTexture2D* InTexture);

	UFUNCTION()
	void OnFilesystemOperation(EFilesystemEventType InType, FString InPath);

	UPROPERTY()
	int UserID = 0;

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	UPROPERTY()
	FString UserHomeDirectory;

	UPROPERTY()
	UPeacegateFileSystem* Filesystem;

public:
	UPROPERTY(BlueprintReadOnly, Category = "Desktop")
	FText TimeOfDay;

	UPROPERTY(BlueprintReadOnly, Category = "Desktop")
	FText NotificationTitle;

	UPROPERTY(BlueprintReadOnly, Category = "Desktop")
	FText NotificationMessage;
	
	UPROPERTY(BlueprintReadOnly, Category = "Desktop")
	UTexture2D* NotificationIcon;

	UPROPERTY(BlueprintReadOnly, Category = "Desktop")
	FString CurrentUsername;

	UPROPERTY(BlueprintReadOnly, Category = "Desktop")
	FString CurrentHostname;

	UPROPERTY(BlueprintReadOnly, Category = "Desktop")
	FString CurrentPeacenetName;

	UPROPERTY(BlueprintReadOnly, Category = "Desktop")
	UTexture2D* WallpaperTexture;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Game Rules")
	FGameRules GameRules;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void ShowProgramOnWorkspace(UProgram* InProgram);

	UFUNCTION(BlueprintImplementableEvent, Category = "Desktop")
	void ClearAppLauncherMainMenu();

	UFUNCTION(BlueprintImplementableEvent, Category = "Desktop")
	void ClearAppLauncherSubMenu();

	UFUNCTION(BlueprintImplementableEvent, Category = "Desktop")
	void AddAppLauncherMainMenuItem(const FText& ItemName);

	UFUNCTION(BlueprintImplementableEvent, Category = "Desktop")
	void AddAppLauncherSubMenuItem(const FText& ItemName, const FText& ItemDescription, const FString& ExecutableName, const UTexture2D* Icon);

protected:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetOpenConnectionCount();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Desktop")
	FString GetIPAddress();

	UPROPERTY()
	TArray<FDesktopNotification> NotificationQueue;

	UFUNCTION(BlueprintCallable, Category = "Desktop")
	void FinishShowingNotification();

	UFUNCTION(BlueprintImplementableEvent, Category = "Desktop")
	void OnShowNotification();

	UFUNCTION(BlueprintCallable, Category = "Desktop")
	void ShowAppLauncherCategory(const FString& InCategory);

	UFUNCTION(BlueprintCallable, Category = "Desktop")
	bool OpenProgram(const FName InExecutableName, UProgram*& OutProgram);

public:
	UFUNCTION()
	void ResetAppLauncher();

	UFUNCTION()
	void EnqueueNotification(const FText& InTitle, const FText& InMessage, UTexture2D* InIcon);

	UFUNCTION()
	void ResetDesktopIcons();


};
