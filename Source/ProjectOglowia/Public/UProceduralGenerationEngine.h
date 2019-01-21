// Copyright (c) 2018 Alkaline Thunder & Bit Phoenix Software

#pragma once

#include "CoreMinimal.h"
#include "FComputer.h"
#include "FPeacenetIdentity.h"
#include "RandomStream.h"
#include "UProceduralGenerationEngine.generated.h"

class APeacenetWorldStateActor;
class UPeacenetSaveGame;

UCLASS()
class PROJECTOGLOWIA_API UProceduralGenerationEngine : public UObject
{
    GENERATED_BODY()

private:
    UPROPERTY()
    APeacenetWorldStateActor* Peacenet;

    UPROPERTY()
    FRandomStream RNG;

public:
    UFUNCTION()
    FComputer& GenerateComputer(FString InHostname, EComputerOwnerType InOwnerType);

    UFUNCTION()
    void Initialize(APeacenetWorldStateActor* InPeacenet);
};