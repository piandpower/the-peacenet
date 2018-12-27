// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "UserWidget.h"
#include "Dialog.h"
#include "USystemContext.h"
#include "UWindow.generated.h"

class UWindow;
class UUserContext;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWindowFocusEvent, bool, IsFocused, UWindow*, Window);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWindowClosedEvent, UWindow*, InWindow);

/**
 * Contains useful functionality for a Peacegate OS window.
 */
UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UWindow : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UUserContext* UserContext;

public: // Getters
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Window")
	UUserContext* GetUserContext();

public: // Setters
	void SetUserContext(UUserContext* InUserContext);
	
public: // Blueprint properties
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	FText WindowTitle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	UTexture2D* Icon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	bool EnableCloseButton = true;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	bool EnableMinimizeAndMaximize = true;

public: // Events
	UPROPERTY(BlueprintAssignable, Category = "Window")
	FWindowClosedEvent NativeWindowClosed;

public: // Functions
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Infobox")
		void ShowInfoWithCallbacks(const FText& InTitle, const FText& InMessage, const EInfoboxIcon InIcon, const EInfoboxButtonLayout ButtonLayout, const bool ShowTextInput, const FInfoboxDismissedEvent& OnDismissed, const FInfoboxInputValidator& ValidatorFunction);

	UFUNCTION(BlueprintCallable, Category = "Infobox")
	void ShowInfo(const FText& InTitle, const FText& InMessage, const EInfoboxIcon InIcon);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "File Management")
	void AskForFile(const FString& InBaseDirectory, const FString& InFilter, const EFileDialogType InDialogType, const FFileDialogDismissedEvent& OnDismissed);

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
	
public:
	FWindowFocusEvent WindowFocusEvent;

	virtual void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;

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
