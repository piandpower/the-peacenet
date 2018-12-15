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

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAddressBookUpdatedEvent);

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
    UPROPERTY(BlueprintAssignable, Category = "Address Book")
    FAddressBookUpdatedEvent EventAddressBookUpdated;

public:
    UFUNCTION()
    void Setup(USystemContext* InSystemContext);

    UFUNCTION()
    void RetrieveContacts();

    UFUNCTION()
    void RemoveContactBackend(int InEntityID, bool InAllowStoryRemoves = false);

    UFUNCTION()
    bool HasContact(int InEntityID);

    UFUNCTION()
    void AddContact(int InEntityID, bool InIsStoryIntegral = true);

    UFUNCTION(BlueprintCallable, Category = "Address Book")
    void RemoveContact(UContact* InContact);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Address Book")
    TArray<UContact*> GetContacts();

    UFUNCTION()
    bool FindComputerAndContact(FPeacenetIdentity InCharacter, FComputer& OutComputer, FPinnedContact& OutContact);

    UFUNCTION()
    bool FindCharacterByID(int InEntityID, FPeacenetIdentity& OutCharacter);
};