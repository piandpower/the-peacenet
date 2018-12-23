// Copyright (c) 2018 Alkaline Thunder & The Peacenet

#pragma once

#include "CoreMinimal.h"
#include "FPinnedContact.h"
#include "FComputer.h"
#include "FContactNote.h"
#include "ECountry.h"
#include "FPeacenetIdentity.h"
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
    int ContactIndex = 0;

    UPROPERTY()
    UAddressBookContext* AddressBook;

public:
    UFUNCTION()
    int GetEntityID();

    UFUNCTION()
    int GetContactIndex();

    UFUNCTION()
    void Setup(UAddressBookContext* InOwnerAddressBook, int InEntityID, int InContactIndex);

public: // Blueprint functions.
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Contact|User Note")
    bool GetUserNotes(TArray<FContactNote>& OutNotes);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Contact")
    bool RetrieveName(FString& OutCharacterName);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Contact")
    bool RetrieveEmail(FString& OutEmail);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Contact")
    bool RetrieveCountry(ECountry& OutCountry);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Contact")
    bool RetrieveReputation(float& OutReputation);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Contact")
    bool RetrieveComputerIP(FString& OutIPAddress);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Contact")
    bool RetrieveComputerHostname(FString& OutHostname);

private:
    UFUNCTION()
    bool FetchCharacter(FPeacenetIdentity& OutCharacter, FComputer& OutComputer, FPinnedContact& OutContact);
};