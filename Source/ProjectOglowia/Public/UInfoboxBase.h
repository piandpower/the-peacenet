// Copyright (c) 2018 Alkaline Thunder & The Peacenet

#pragma once

#include "CoreMinimal.h"
#include "Text.h"
#include "Dialog.h"
#include "UProgram.h"
#include "UInfoboxBase.generated.h"

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
	void BindCallbacks(const FInfoboxDismissedEvent InDismissedEvent, const FInfoboxInputValidator InTextValidator);
	
	UFUNCTION(BlueprintCallable, Category = "Infobox Base")
	void Dismiss(const EDialogResult InDialogResult, const FText& InUserInputText);

	UFUNCTION(BlueprintCallable, Category = "Infobox Base")
	void ValidateTextInput(const FText& InText);
};