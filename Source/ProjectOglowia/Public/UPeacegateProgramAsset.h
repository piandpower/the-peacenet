// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "UWindow.h"
#include "SystemContext.h"
#include "UPeacegateProgramAsset.generated.h"

class UConsoleContext;
class UPTerminalWidget;

UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UProgram : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Peacegate")
	static UProgram* CreateProgram(const TSubclassOf<UWindow> InWindowClass, const TSubclassOf<UProgram> InProgramClass, const TScriptInterface<ISystemContext> InSystem, const int InUserID, UWindow*& OutWindow);

	UFUNCTION(BlueprintCallable, Category = "Peacegate")
	UConsoleContext* CreateConsole(UPARAM(Ref) UPTerminalWidget* InTerminalWidget);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Wallpaper")
	UWallpaperAsset* GetWallpaper();

	UFUNCTION(BlueprintCallable, Category = "Wallpaper")
	void SetWallpaper(FName InWallpaperID);

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

protected:
	// Filesystem context for the program.
	UPROPERTY(BlueprintReadOnly, Category = "Program")
	class UPeacegateFileSystem* Filesystem;

	// The console allows the program to output to a Terminal, or run Terminal Commands as its user.
	UPROPERTY(BlueprintReadOnly, Category = "Program")
	UConsoleContext* Console;
};

USTRUCT(BlueprintType)
struct FAppLauncherItemInformation
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Category;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTexture2D* Icon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool EnableMinimizeAndMaximize = true;

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