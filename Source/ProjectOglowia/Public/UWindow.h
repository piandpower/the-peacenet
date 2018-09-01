// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "UserWidget.h"
#include "UWindow.generated.h"

/**
 * Contains useful functionality for a Peacegate OS window.
 */
UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UWindow : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	FText WindowTitle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	UTexture2D* Icon;
	
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
