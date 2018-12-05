// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "UserWidget.h"
#include "UWindow.h"
#include "UWorkspace.generated.h"

class UProgram;

/**
 * Represents a workspace in a Peacenet desktop.
 */
UCLASS(Blueprintable, Abstract)
class PROJECTOGLOWIA_API UWorkspace : public UUserWidget
{
	GENERATED_BODY()


protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Workspace")
	TArray<UWindow*> GetWindows();

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Workspace")
	void OnWindowAdded(UWindow* InWindow);
	
	UFUNCTION()
	void OnWindowClosed(UWindow* InWindow);

	UFUNCTION()
	void ShowProgramOnWorkspace(UProgram* InProgram);

	UPROPERTY()
	UWindow* ActiveWindow = nullptr;

	UFUNCTION()
	void WindowFocusEvent(bool IsFocused, UWindow* InWindow);
};
