// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "UWindow.h"
#include "SystemContext.h"
#include "UPeacegateProgramAsset.generated.h"

UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UProgram : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	UWindow* Window;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	TScriptInterface<ISystemContext> SystemContext;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	int UserID;

	UFUNCTION(BlueprintCallable)
		void SetWindowMinimumSize(FVector2D InSize)
	{
		Window->SetClientMinimumSize(InSize);
	}

	UFUNCTION(BlueprintCallable)
	void InjectIntoWindow()
	{
		Window->SystemContext = this->SystemContext;
		Window->UserID = this->UserID;
		Window->AddWindowToClientSlot(this);
	}
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