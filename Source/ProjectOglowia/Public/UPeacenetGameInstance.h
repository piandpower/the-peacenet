// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UPeacenetSettings.h"
#include "UPeacenetGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSettingsAppliedEvent, UPeacenetSettings*, InSettings);

/**
 * A game instance specifically for Peacenet.
 */
UCLASS(BlueprintType)
class PROJECTOGLOWIA_API UPeacenetGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// The settings for the current instance of The Peacenet.
	UPROPERTY()
	UPeacenetSettings* Settings;

	UPROPERTY(BlueprintAssignable, Category = "Settings")
	FSettingsAppliedEvent SettingsApplied;

	UFUNCTION(BlueprintCallable, Category = "Settings")
	UPeacenetSettings* GetSettings();

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SaveSettings();

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void LoadSettings();

public: // UGameInstance overrides.
	virtual void Init() override;
	virtual void Shutdown() override;
};
