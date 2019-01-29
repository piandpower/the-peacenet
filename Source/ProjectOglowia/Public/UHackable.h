// Copyright (c) 2019 Alkaline Thunder & Bit Phoenix Software.

#pragma once

#include "CoreMinimal.h"
#include "FComputer.h"
#include "FPeacenetIdentity.h"
#include "FFirewallRule.h"
#include "EConnectionError.h"
#include "UHackable.generated.h"

class UComputerService;
class UPeacenetSaveGame;
class USystemContext;
class UUserContext;
class APeacenetWorldStateActor;
class UPeacegateFileSystem;

UCLASS(Blueprintable, BlueprintType, Abstract)
class PROJECTOGLOWIA_API UHackable : public UObject
{
    GENERATED_BODY()

private:
    UPROPERTY()
    UComputerService* Service;

    UPROPERTY()
    UUserContext* OriginUserContext;

    UPROPERTY()
    USystemContext* RemoteSystem;

public:
    UFUNCTION()
    void SetRemoteSystem(USystemContext* InSystem);

    UFUNCTION()
    void SetOriginUser(UUserContext* InOriginUser);

    UFUNCTION()
    void SetService(UComputerService* InService);

public:
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Hackable")
    static FString GetConnectionErrorText(EConnectionError InError);

    UFUNCTION(BlueprintCallable, Category = "Hackable")
    static bool OpenConnection(FString InHost, int InPort, UComputerService* TargetServiceType, UUserContext* OriginUser, EConnectionError& OutError, UHackable*& OutConnection);
};