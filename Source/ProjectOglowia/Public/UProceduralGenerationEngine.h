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

public:
    UFUNCTION()
    void Initialize(APeacenetWorldStateActor* InPeacenet);
};