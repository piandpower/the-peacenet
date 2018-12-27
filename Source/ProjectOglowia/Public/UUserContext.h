// Copyright (c) 2018 Alkaline Thunder & The Peacenet

#pragma once

#include "CoreMinimal.h"
#include "USystemContext.h"
#include "UUserContext.generated.h"

/**
 * A System Context that acts as a specific user.
 */
UCLASS(BlueprintType)
class PROJECTOGLOWIA_API UUserContext : public UObject
{
    GENERATED_BODY()

private:
    // The owning system context.
    UPROPERTY()
    USystemContext* OwningSystem = nullptr;

    // The user ID of the user who owns this system context.
    UPROPERTY()
    int UserID = 0;

public:
    UFUNCTION()
    void Setup(USystemContext* InOwningSystem, int InUserID);
};