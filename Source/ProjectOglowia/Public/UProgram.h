// Copyright (c) 2018 Alkaline Thunder & The Peacenet.

#pragma once

#include "CoreMinimal.h"
#include "Text.h"
#include "Dialog.h"
#include "Blueprint/UserWidget.h"
#include "UProgram.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerAttentionNeededEvent, bool, PlaySound);

class UUserContext;
class USystemContext;
class UAddressBookContext;
class UConsoleContext;
class UWindow;

UCLASS(Blueprintable, BlueprintType)
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
	UFUNCTION(BlueprintCallable, Category = "Desktop")
	void PushNotification(const FText& InNotificationMessage);

	UFUNCTION(BlueprintCallable, Category = "Program")
	void RequestPlayerAttention(bool PlaySound);

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Program")
	UUserContext* GetUserContext();

	UPROPERTY(BlueprintAssignable, Category = "Program")
	FPlayerAttentionNeededEvent PlayerAttentionNeeded;

	UFUNCTION(BlueprintCallable, Category = "Peacegate")
	static UProgram* CreateProgram(const TSubclassOf<UWindow> InWindowClass, const TSubclassOf<UProgram> InProgramClass, USystemContext* InSystem, const int InUserID, UWindow*& OutWindow, bool DoContextSetup = true);

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

	// The console allows the program to output to a Terminal, or run Terminal Commands as its user.
	UPROPERTY(BlueprintReadOnly, Category = "Program")
	UConsoleContext* Console;
};
