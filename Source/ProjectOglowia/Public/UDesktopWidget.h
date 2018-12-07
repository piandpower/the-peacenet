// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "UserWidget.h"
#include "UPeacegateProgramAsset.h" 
#include "UPeacegateFileSystem.h"
#include "FEventLogEntry.h"
#include "UDesktopWidget.generated.h"

class UWorkspace;
class USystemContext;
class UConsoleContext;
class UPTerminalWidget;
class UNetMapWidget;
class UImageLoader;

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

private:
	void ResetEventLog();

protected:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "System")
	USystemContext* GetSystemContext();

public:
	UFUNCTION()
	void SelectCharacterNode(int InEntityID);

	UFUNCTION(BlueprintImplementableEvent, Category = "System")
	void ExecuteCommand(const FString& InCommand);

	void ResetNetMap();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "System")
	void SwitchWorkspace(int InWorkspaceNumber);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "System")
	void MoveActiveProgramToWorkspace(int InWorkspaceNumber);

	UFUNCTION(BlueprintCallable, Category = "System")
	void CloseActiveProgram();

	FActiveProgramCloseEvent EventActiveProgramClose;

protected:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Missions")
	bool IsInMission();

	UFUNCTION(BlueprintCallable, Category = "Desktop")
	UProgram* SpawnProgramFromClass(TSubclassOf<UProgram> InClass, const FText& InTitle, UTexture2D* InIcon);

	UFUNCTION(BlueprintImplementableEvent, Category = "NetMap")
	void CharacterNodeSelected(const FPeacenetIdentity& InIdentity, const FComputer& InComputer);

	UFUNCTION(BlueprintCallable, Category = "NetMap")
	UNetMapWidget* CreateNetMap(TSubclassOf<UNetMapWidget> InSubclass);

	UPROPERTY(BlueprintReadOnly, Category = "NetMap")
	UNetMapWidget* NetMap;

	UPROPERTY(BlueprintReadOnly, Category = "Desktop")
	FPeacenetIdentity MyCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Desktop")
	FComputer MyComputer;

private:
	bool bIsWaitingForNotification = false;

protected:
	UFUNCTION(BlueprintCallable, Category = "Console")
	UConsoleContext* CreateConsole(UPTerminalWidget* InTerminal);

protected:
	UPROPERTY()
	UImageLoader* ImageLoader;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Missions")
	void OnMissionCompleted(const UMissionAsset* InMission, const USystemContext* InMissionContext, const TArray<UMissionUnlock*>& InMissionUnlocks);

	UFUNCTION(BlueprintImplementableEvent, Category = "Missions")
	void OnMissionFailed(const UMissionAsset* InMission, const USystemContext* InMissionContext, const FString& InFailReason);

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

	UFUNCTION(BlueprintImplementableEvent, Category = "Event Log")
		void OnClearEventLog();

	UFUNCTION(BlueprintImplementableEvent, Category = "Event Log")
	void OnAddEventToLog(FEventLogEntry InEvent);

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
	FText CurrentUsername;

	UPROPERTY(BlueprintReadOnly, Category = "Desktop")
	FText CurrentHostname;

	UPROPERTY(BlueprintReadOnly, Category = "Desktop")
	FText CurrentPeacenetName;

	UPROPERTY(BlueprintReadOnly, Category = "Desktop")
	UTexture2D* WallpaperTexture;

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "System")
	UWorkspace* GetCurrentWorkspace();

	UFUNCTION()
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
	UPROPERTY()
	TArray<FDesktopNotification> NotificationQueue;

	UFUNCTION(BlueprintCallable, Category = "Desktop")
	void FinishShowingNotification();

	UFUNCTION(BlueprintImplementableEvent, Category = "Desktop")
	void OnShowNotification();

	UFUNCTION(BlueprintCallable, Category = "Desktop")
	void ShowAppLauncherCategory(const FString& InCategory);

	UFUNCTION(BlueprintCallable, Category = "Desktop")
	void OpenProgram(const FName InExecutableName);

public:
	UFUNCTION()
	void ResetAppLauncher();

	UFUNCTION()
	void EnqueueNotification(const FText& InTitle, const FText& InMessage, UTexture2D* InIcon);

	UFUNCTION()
	void ResetWindowList();
	
	UFUNCTION()
	void ResetDesktopIcons();


};
