// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "UserWidget.h"
#include "USystemContext.h"
#include "UWindow.generated.h"

UENUM(BlueprintType)
enum class EDialogResult : uint8
{
	OK,
	Cancel,
	Yes,
	No
};

UENUM(BlueprintType)
enum class EInfoboxButtonLayout : uint8
{
	OK,
	OKCancel,
	YesNo,
	YesNoCancel
};

UENUM(BlueprintType)
enum class EInfoboxIcon : uint8
{
	Info,
	Error,
	Question,
	Warning
};

UENUM(BlueprintType)
enum class EFileDialogType : uint8
{
	Open,
	Save
};

DECLARE_DYNAMIC_DELEGATE_TwoParams(FInfoboxDismissedEvent, EDialogResult, DialogResult, FText, UserTextInput);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FInfoboxInputValidator, FText, UserTextInput, FText&, OutError);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FFileDialogDismissedEvent, bool, FileSelected, FString, FilePath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWindowClosedEvent);

/**
 * Contains useful functionality for a Peacegate OS window.
 */
UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UWindow : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Window")
	FWindowClosedEvent NativeWindowClosed;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Infobox")
		void ShowInfoWithCallbacks(const FText& InTitle, const FText& InMessage, const EInfoboxIcon InIcon, const EInfoboxButtonLayout ButtonLayout, const bool ShowTextInput, const FInfoboxDismissedEvent& OnDismissed, const FInfoboxInputValidator& ValidatorFunction);

	UFUNCTION(BlueprintCallable, Category = "Infobox")
	void ShowInfo(const FText& InTitle, const FText& InMessage, const EInfoboxIcon InIcon);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "File Management")
	void AskForFile(const FString& InBaseDirectory, const FString& InFilter, const EFileDialogType InDialogType, const FFileDialogDismissedEvent& OnDismissed);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	FText WindowTitle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	UTexture2D* Icon;

	UPROPERTY(BlueprintReadOnly)
	int UserID;

	UPROPERTY(BlueprintReadOnly)
	USystemContext* SystemContext;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	bool EnableCloseButton = true;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	bool EnableMinimizeAndMaximize = true;

	UFUNCTION(BlueprintCallable)
	void Close();
	
	UFUNCTION(BlueprintCallable)
	void Minimize();

	UFUNCTION(BlueprintCallable)
	void Maximize();
	
	UFUNCTION(BlueprintCallable)
	void Restore();

	void AddWindowToClientSlot(const UUserWidget* InClientWidget);

	void SetClientMinimumSize(const FVector2D& InSize);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnWindowClosed();

	UFUNCTION(BlueprintImplementableEvent)
		void OnWindowMinimized();

	UFUNCTION(BlueprintImplementableEvent)
	void OnAddWindowToClientSlot(const UUserWidget* InClientWidget);


	UFUNCTION(BlueprintImplementableEvent)
		void OnWindowMaximized();

	UFUNCTION(BlueprintImplementableEvent)
		void OnWindowRestored();

	UFUNCTION(BlueprintImplementableEvent)
	void OnSetClientMinimumSize(const FVector2D& InSize);
};
