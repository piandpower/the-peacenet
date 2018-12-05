// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "UWorkspace.h"
#include "UMG/Public/Components/HorizontalBox.h"
#include "UMG/Public/Components/VerticalBox.h"
#include "HorizontalBox.h"
#include "UWindow.h"
#include "VerticalBox.h"
#include "UTilerWorkspaceBase.generated.h"

UENUM(BlueprintType)
enum class ETilerSplitType : uint8
{
	Horizontal,
	Vertical
};

/**
 * Base functionality for the Peacenet Tiler Workspace (faux i3wm).
 */
UCLASS(BlueprintType, Blueprintable)
class PROJECTOGLOWIA_API UTilerWorkspaceBase : public UWorkspace
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Window Tiler")
	ETilerSplitType CurrentSplitType = ETilerSplitType::Horizontal;

private:
	bool IsBox(UPanelWidget* InWidget);

protected:
	UFUNCTION(BlueprintCallable, Category = "Window Tiler")
	bool RemoveEmptyBoxes(UPanelWidget* InWidget);

	UFUNCTION(BlueprintCallable, Category = "Window Tiler")
	void NativeHandleOpenWindow(UWindow* InWindow);

	UFUNCTION(BlueprintImplementableEvent, Category = "Window Tiler")
	UPanelWidget* GetTilerRootPanel();
};
