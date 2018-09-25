// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "UserWidget.h"
#include "SystemContext.h"
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

DECLARE_DYNAMIC_DELEGATE_TwoParams(FInfoboxDismissedEvent, EDialogResult, DialogResult, FText, UserTextInput);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FInfoboxInputValidator, FText, UserTextInput, FText&, OutError);

/**
 * Contains useful functionality for a Peacegate OS window.
 */
UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UWindow : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Infobox")
		void ShowInfoWithCallbacks(const FText& InTitle, const FText& InMessage, const EInfoboxIcon InIcon, const EInfoboxButtonLayout ButtonLayout, const bool ShowTextInput, const FInfoboxDismissedEvent& OnDismissed, const FInfoboxInputValidator& ValidatorFunction);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Infobox")
	void ShowInfo(const FText& InTitle, const FText& InMessage, const EInfoboxIcon InIcon);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	FText WindowTitle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	UTexture2D* Icon;

	UPROPERTY(BlueprintReadOnly)
	int UserID;

	UPROPERTY(BlueprintReadOnly)
	TScriptInterface<ISystemContext> SystemContext;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	bool EnableCloseButton = true;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	bool EnableMinimizeAndMaximize = true;

	UFUNCTION(BlueprintCallable)
		void Close()
	{
		OnWindowClosed();
	}

	UFUNCTION(BlueprintCallable)
		void Minimize()
	{
		OnWindowMinimized();
	}

	UFUNCTION(BlueprintCallable)
		void Maximize()
	{
		OnWindowMaximized();
	}

	UFUNCTION(BlueprintCallable)
		void Restore()
	{
		OnWindowRestored();
	}

	void AddWindowToClientSlot(const UUserWidget* InClientWidget)
	{
		OnAddWindowToClientSlot(InClientWidget);
	}

	void SetClientMinimumSize(const FVector2D& InSize)
	{
		OnSetClientMinimumSize(InSize);
	}

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
