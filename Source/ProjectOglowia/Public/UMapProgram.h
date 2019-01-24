// Copyright (c) 2019 Alkaline Thunder & Bit Phoenix Software

#pragma once

#include "CoreMinimal.h"
#include "UProgram.h"
#include "UUserContext.h"
#include "FPeacenetIdentity.h"
#include "UMapProgram.generated.h"

class UMapNodeContext;

UCLASS(Blueprintable, BlueprintType, Abstract)
class PROJECTOGLOWIA_API UMapProgram : public UProgram
{
    GENERATED_BODY()

private:
    UPROPERTY()
    bool SpawnedLinksYet = false;

    UPROPERTY()
    TArray<int> NodeIDsToSpawn;

    UPROPERTY()
    TArray<UMapNodeContext*> LoadedContexts;

protected:
    UFUNCTION()
    UMapNodeContext* GetContext(int InEntityID);

    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    UFUNCTION(BlueprintImplementableEvent, Category = "Map")
    void ClearNodes();

    UFUNCTION(BlueprintImplementableEvent, Category = "Map")
    void ConnectNodes(UMapNodeContext* LeftLink, UMapNodeContext* RightLink);

    UFUNCTION(BlueprintImplementableEvent, Category = "Map")
    void AddNode(UMapNodeContext* InNodeContext);
};
