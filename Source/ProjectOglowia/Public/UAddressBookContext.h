// Copyright (c) 2018 Alkaline Thunder & The Peacenet

#pragma once

#include "CoreMinimal.h"
#include "UPeacenetSaveGame.h"
#include "FPeacenetIdentity.h"
#include "FComputer.h"
#include "ECountry.h"
#include "UContact.h"
#include "PeacenetWorldStateActor.h"
#include "UAddressBookContext.generated.h"

// Forward declarations for circular dependencies...
class USystemContext;

/**
 * Encapsulates a Peacenet address book.
 */
UCLASS(BlueprintType)
class PROJECTOGLOWIA_API UAddressBookContext : public UObject
{
    GENERATED_BODY()

protected:
    UPROPERTY()
    USystemContext* SystemContext;

    UPROPERTY()
    TArray<UContact*> Contacts;

public:
    UFUNCTION()
    void Setup(USystemContext* InSystemContext);

    UFUNCTION()
    void RetrieveContacts();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Address Book")
    TArray<UContact*> GetContacts();
};