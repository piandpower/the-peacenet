// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "UPeacenetSettings.generated.h"

UENUM(BlueprintType)
enum class EGUIScale : uint8
{
	Normal,
	Medium,
	Large
};

/**
 * A save game object that contains all of Peacenet's settings.
 */
UCLASS(BlueprintType)
class PROJECTOGLOWIA_API UPeacenetSettings : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|UI")
	EGUIScale GUIScale = EGUIScale::Normal;
};
