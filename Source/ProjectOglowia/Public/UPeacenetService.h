// Copyright (c) 2018 Alkaline Thunder & The Peacenet

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UPeacenetService.generated.h"

class USystemContext;
class UPeacenetServiceAsset;

/**
 * Encapsulates a Peacenet service and acts as a base for all service pages.
 */
UCLASS(Blueprintable, BlueprintType, Abstract)
class PROJECTOGLOWIA_API UPeacenetService : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY()
    UPeacenetServiceAsset* ServiceAsset;

    UPROPERTY()
    USystemContext* SystemContext;

public:
    UFUNCTION()
    USystemContext* GetSystemContext();

    UFUNCTION()
    void Setup(USystemContext* InSystemContext, UPeacenetServiceAsset* InServiceAsset);

};