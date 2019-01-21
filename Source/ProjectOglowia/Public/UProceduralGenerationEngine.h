// Copyright (c) 2018 Alkaline Thunder & Bit Phoenix Software

#pragma once

#include "CoreMinimal.h"
#include "FComputer.h"
#include "FPeacenetIdentity.h"
#include "ECountry.h"
#include "RandomStream.h"
#include "UProceduralGenerationEngine.generated.h"

class UMarkovChain;
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

    UPROPERTY()
    UMarkovChain* MaleNameGenerator;

    UPROPERTY()
    UMarkovChain* FemaleNameGenerator;

public:
    UFUNCTION()
    FComputer& GenerateComputer(FString InHostname, EComputerOwnerType InOwnerType);

    UFUNCTION()
    FString GenerateIPAddress(ECountry InCountry);

    UFUNCTION()
    void Initialize(APeacenetWorldStateActor* InPeacenet);
};