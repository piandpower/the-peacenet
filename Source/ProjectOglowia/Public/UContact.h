// Copyright (c) 2018 Alkaline Thunder & The Peacenet

#pragma once

#include "CoreMinimal.h"
#include "UContact.generated.h"

class UAddressBookContext;

/**
 * Encapsulates a Peacenet contact.
 */
UCLASS(BlueprintType)
class PROJECTOGLOWIA_API UContact : public UObject
{
    GENERATED_BODY()

protected:
    UPROPERTY()
    int EntityID = 0;

    UPROPERTY()
    UAddressBookContext* AddressBook;

public:
    UFUNCTION()
    int GetEntityID();

    UFUNCTION()
    void Setup(UAddressBookContext* InOwnerAddressBook, int InEntityID);
};