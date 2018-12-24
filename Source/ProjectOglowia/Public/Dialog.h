// Copyright (c) 2018 Alkaline Thunder & The Peacenet

#pragma once

#include "CoreMinimal.h"
#include "Text.h"
#include "Dialog.generated.h"

UENUM(BlueprintType)
enum class EFileDialogType : uint8
{
	Open,
	Save
};


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

DECLARE_DYNAMIC_DELEGATE_TwoParams(FFileDialogDismissedEvent, bool, FileSelected, FString, FilePath);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FInfoboxDismissedEvent, EDialogResult, DialogResult, FText, UserTextInput);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FInfoboxInputValidator, FText, UserTextInput, FText&, OutError);