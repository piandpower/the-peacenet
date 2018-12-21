// Copyright (c) 2018 Alkaline Thunder & The Peacenet

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UPeacenetService.generated.h"

class USystemContext;
class UPeacenetServiceAsset;
class UContact;
class UAddressBookContext;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPeacenetServiceContactOpenRequestedEvent, UContact*, InContact);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPeacenetServiceNavigateRequestedEvent, FString, InURL);

/**
 * Encapsulates a Peacenet service and acts as a base for all service pages.
 */
UCLASS(Blueprintable, BlueprintType, Abstract)
class PROJECTOGLOWIA_API UPeacenetService : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY()
    UPeacenetServiceAsset* ServiceAsset;

    UPROPERTY()
    USystemContext* SystemContext;

protected:
    UFUNCTION(BlueprintCallable, Category = "Address Book")
    void OpenPeacenetIdentity(int InEntityID, UContact*& OutContact);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Peacenet Service|Address Book")
    UAddressBookContext* GetAddressBook();

    UFUNCTION(BlueprintCallable, Category = "Peacenet Service|Navigation")
    void NavigateToURL(FString InURL);

public:
    UPROPERTY(BlueprintAssignable, Category = "Peacenet Service|Navigation")
    FPeacenetServiceNavigateRequestedEvent EventOnNavigationRequested;

    UPROPERTY(BlueprintAssignable, Category = "Peacenet Service|Address Book")
    FPeacenetServiceContactOpenRequestedEvent EventOnContactOpenRequested;

    UFUNCTION()
    USystemContext* GetSystemContext();

    UFUNCTION()
    void Setup(USystemContext* InSystemContext, UPeacenetServiceAsset* InServiceAsset);

};