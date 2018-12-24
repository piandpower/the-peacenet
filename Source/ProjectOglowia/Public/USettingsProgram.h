// Copyright (c) 2018 Alkaline Thunder & The Peacenet

#pragma once

#include "CoreMinimal.h"
#include "UProgram.h"
#include "USettingsProgram.generated.h"

class UDesktopWidget;

UCLASS(Blueprintable)
class PROJECTOGLOWIA_API USettingsProgram : public UProgram
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Settings Program")
	UDesktopWidget* Desktop;

	virtual void NativeProgramLaunched() override;
};