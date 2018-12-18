// Copyright (c) 2018 Alkaline Thunder & The Peacenet

#pragma once

#include "CoreMinimal.h"
#include "UPeacenetServiceAsset.h"
#include "UPeacenetServiceUtils.generated.h"

class UProgram;
class UPeacenetService;

/**
 * Blueprint function library for Peacenet Service utilities.
 */
UCLASS(BlueprintType)
class PROJECTOGLOWIA_API UPeacenetServiceUtils : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Peacenet Service Utilities")
    static bool GetService(FString InServiceURL, UPeacenetServiceAsset*& OutService);

    UFUNCTION(BlueprintCallable, Category = "Peacenet Service Utilities", meta=(DefaultToSelf = "InOwningProgram"))
    static UPeacenetService* CreateServiceWidget(UProgram* InOwningProgram, UPeacenetServiceAsset* InService);
};