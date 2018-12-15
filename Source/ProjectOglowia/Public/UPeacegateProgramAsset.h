// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "UWindow.h"
#include "USystemContext.h"
#include "Engine/DataAsset.h"
#include "FNetMapScanEventArgs.h"
#include "UPeacegateProgramAsset.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerAttentionNeededEvent, bool, PlaySound);

class UAddressBookContext;
class UConsoleContext;
class UPTerminalWidget;
class UMissionAsset;
class APeacenetWorldStateActor;

UENUM(BlueprintType)
enum class EProgramFileOpenStatus : uint8
{
	OK,
	FileNotFound,
	PermissionDenied,
	NoSuitableProgram
};

UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UProgram : public UUserWidget
{
	GENERATED_BODY()

private:
	// Learned this hack from the ShiftOS Visual Basic days.
	bool JustOpened = true;

public:
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

	UFUNCTION()
		void ActiveProgramCloseEvent();

protected:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Address Book")
	UAddressBookContext* GetAddressBook();

	UFUNCTION(BlueprintCallable, Category = "Missions")
	void StartMission(UMissionAsset* InMissionAsset);

	UFUNCTION(BlueprintCallable, Category = "Desktop")
		void PushNotification(const FText& InNotificationMessage);

	UFUNCTION(BlueprintImplementableEvent, Category = "NetMap")
	void NetMapScan(int InEntityID, FNetMapScanEventArgs EventArgs);

	UFUNCTION(BlueprintCallable, Category = "Program")
	void RequestPlayerAttention(bool PlaySound);

public:
	UPROPERTY(BlueprintAssignable, Category = "Program")
	FPlayerAttentionNeededEvent PlayerAttentionNeeded;

	UFUNCTION(BlueprintCallable, Category = "System")
	void ExecuteCommand(FString InCommand);

	UFUNCTION(BlueprintCallable, Category = "Peacegate")
	static UProgram* CreateProgram(const TSubclassOf<UWindow> InWindowClass, const TSubclassOf<UProgram> InProgramClass, USystemContext* InSystem, const int InUserID, UWindow*& OutWindow, bool DoContextSetup = true);
	
	UFUNCTION(BlueprintCallable, Category = "Missions")
	bool GetAvailableMissions(TArray<UMissionAsset*>& OutMissions);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Missions")
	bool IsMissionActive();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Missions")
	bool MissionsEnabled();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Peacegate")
	FText GetUsername();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Peacegate")
	FString HomeDirectory();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Peacegate")
	FText GetHostname();

	UFUNCTION(BlueprintCallable, Category = "Peacegate")
	UConsoleContext* CreateConsole(UPARAM(Ref) UPTerminalWidget* InTerminalWidget);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	UWindow* Window;

	UFUNCTION(BlueprintCallable, Category = "Infobox")
	void ShowInfoWithCallbacks(const FText& InTitle, const FText& InMessage, const EInfoboxIcon InIcon, const EInfoboxButtonLayout ButtonLayout, const bool ShowTextInput, const FInfoboxDismissedEvent& OnDismissed, const FInfoboxInputValidator& ValidatorFunction);
	
	UFUNCTION(BlueprintCallable, Category = "Infobox")
	void ShowInfo(const FText& InTitle, const FText& InMessage, const EInfoboxIcon InIcon);

	UFUNCTION(BlueprintCallable, Category = "File Management")
	void AskForFile(const FString InBaseDirectory, const FString InFilter, const EFileDialogType InDialogType, const FFileDialogDismissedEvent& OnDismissed);
	
	void SetupContexts();

	UFUNCTION(BlueprintCallable)
	void SetWindowMinimumSize(FVector2D InSize);

	UFUNCTION(BlueprintImplementableEvent, Category = "Program")
	void FileOpened(const FString& InPath);
protected:
	UFUNCTION()
	virtual void NativeProgramLaunched();

	// Filesystem context for the program.
	UPROPERTY(BlueprintReadOnly, Category = "Program")
	class UPeacegateFileSystem* Filesystem;

	// The console allows the program to output to a Terminal, or run Terminal Commands as its user.
	UPROPERTY(BlueprintReadOnly, Category = "Program")
	UConsoleContext* Console;

	UFUNCTION(BlueprintCallable, Category = "Program")
	bool OpenFile(const FString& InPath, EProgramFileOpenStatus& OutStatus);
};

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
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName ExecutableName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FAppLauncherItemInformation AppLauncherItem;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UProgram> ProgramClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool IsUnlockedByDefault = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FString> SupportedFileExtensions;
};

UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UInfoboxBase : public UProgram
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Infobox Base", meta = (ExposeOnSpawn = "true"))
	FText Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Infobox Base", meta = (ExposeOnSpawn = "true"))
	FText Message;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Infobox Base", meta = (ExposeOnSpawn = "true"))
	EInfoboxIcon Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Infobox Base", meta = (ExposeOnSpawn = "true"))
	EInfoboxButtonLayout ButtonLayout;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Infobox Base", meta = (ExposeOnSpawn = "true"))
	bool ShowTextInputField;

	UPROPERTY(BlueprintReadOnly, Category = "Infobox Base")
	FText TextInputErrorText;

private:
	FInfoboxDismissedEvent DismissCallback;
	FInfoboxInputValidator Validator;

public:
	UFUNCTION(BlueprintCallable, Category="Infobox Base")
	void BindCallbacks(const FInfoboxDismissedEvent InDismissedEvent, const FInfoboxInputValidator InTextValidator)
	{
		DismissCallback = InDismissedEvent;
		Validator = InTextValidator;
	}
	
	UFUNCTION(BlueprintCallable, Category = "Infobox Base")
	void Dismiss(const EDialogResult InDialogResult, const FText& InUserInputText)
	{
		Window->Close();
		DismissCallback.ExecuteIfBound(InDialogResult, InUserInputText);
	}

	UFUNCTION(BlueprintCallable, Category = "Infobox Base")
		void ValidateTextInput(const FText& InText)
	{
		if (Validator.IsBound())
		{
			Validator.Execute(InText, TextInputErrorText);
		}
		else
		{
			TextInputErrorText = FText::GetEmpty();
		}
	}
};


UCLASS(Blueprintable)
class PROJECTOGLOWIA_API USettingsProgram : public UProgram
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Settings Program")
	UDesktopWidget* Desktop;

	virtual void NativeProgramLaunched() override;
};