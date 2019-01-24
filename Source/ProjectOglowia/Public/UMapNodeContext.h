// Copyright (c) 2019 Alkaline Thunder & Bit Phoenix Software.

#pragma once

#include "CoreMinimal.h"
#include "FPeacenetIdentity.h"
#include "UMapNodeContext.generated.h"

class UMapProgram;
class UPeacenetSaveGame;

UCLASS(BlueprintType)
class PROJECTOGLOWIA_API UMapNodeContext : public UObject
{
    GENERATED_BODY()

private:
    UPROPERTY()
    UMapProgram* MapProgram;

    UPROPERTY()
    int NodeID = 0;

protected:
    UFUNCTION()
    FPeacenetIdentity& GetIdentity();

    UFUNCTION()
    UPeacenetSaveGame* GetSaveGame();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Map Node")
    FVector2D GetPosition();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Map Node")
    FString GetNodeName();

public:
    UFUNCTION()
    int GetNodeID();

    UFUNCTION()
    void Setup(UMapProgram* InMapProgram, int InNodeID);
};