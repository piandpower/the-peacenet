// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SettingsManager.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class USettingsManager : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTOGLOWIA_API ISettingsManager
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Settings|Audio")
	float GetUISoundVolume();

	UFUNCTION(BlueprintImplementableEvent, Category = "Settings|Audio")
	float GetMusicVolume();

	UFUNCTION(BlueprintImplementableEvent, Category = "Settings|Audio")
	bool GetEnableAmbientMusic();

	UFUNCTION(BlueprintImplementableEvent, Category = "Settings|UI")
	bool GetEnableWindowAnimations();

	UFUNCTION(BlueprintImplementableEvent, Category = "Settings|UI")
	bool GetEnableDesktopAnimations();

	UFUNCTION(BlueprintImplementableEvent, Category = "Settings|UI")
	bool GetEnableWidgetAnimations();

	UFUNCTION(BlueprintImplementableEvent, Category = "Settings|Windows")
	bool GetEnableWindowGrayout();

	UFUNCTION(BlueprintImplementableEvent, Category = "Settings|Windows")
		bool GetEnableWindowRedAsRoot();

	UFUNCTION(BlueprintImplementableEvent, Category = "Settings|Effects")
		bool GetEnableBloom();

	UFUNCTION(BlueprintImplementableEvent, Category = "Settings|Effects")
		bool GetEnableCRTInterlace();

	UFUNCTION(BlueprintImplementableEvent, Category = "Settings|Effects")
		bool GetEnableAlertRedshift();

	UFUNCTION(BlueprintImplementableEvent, Category = "Settings|Effects")
		bool GetEnableAudioReactiveBloom();
};
