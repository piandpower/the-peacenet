// Copyright (c) 2019 Alkaline Thunder & Bit Phoenix Software.

#pragma once

#include "CoreMinimal.h"
#include "FComputer.h"
#include "FPeacenetIdentity.h"
#include "FFirewallRule.h"
#include "EConnectionError.h"
#include "EAuthenticationType.h"
#include "UHackable.generated.h"

class UHackable;

DECLARE_DYNAMIC_DELEGATE_TwoParams(FAuthenticationRequiredEvent, EAuthenticationType, AuthType, UHackable*, Hackable);

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

protected:
    UFUNCTION()
    void CompleteHack();

    UFUNCTION(BlueprintImplementableEvent, Category = "Hackable")
    void HackCompleted();

    virtual void NativeHackCompleted();

public:
    UFUNCTION()
    void SetRemoteSystem(USystemContext* InSystem);

    UFUNCTION()
    void SetOriginUser(UUserContext* InOriginUser);

    UFUNCTION()
    void SetService(UComputerService* InService);

    UFUNCTION()
    void StartAuth(FAuthenticationRequiredEvent InCallback);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Networking")
    EAuthenticationType GetAuthenticationType();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Networking")
    bool NeedsAuthentication();

    UFUNCTION(BlueprintCallable, Category = "Networking")
    bool AuthenticateWithPassword(FString InPassword);
    
    UFUNCTION(BlueprintCallable, Category = "Networking")
    bool AuthenticateWithUsernameAndPassword(FString InUsername, FString InPassword);

    UFUNCTION(BlueprintCallable, Category = "Networking")
    bool AuthenticateWithPrivateKeyFile(FString InPrivateKeyPath);

public:
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Hackable")
    static FString GetConnectionErrorText(EConnectionError InError);

    UFUNCTION(BlueprintCallable, Category = "Hackable")
    static bool OpenConnection(FString InHost, int InPort, UComputerService* TargetServiceType, UUserContext* OriginUser, EConnectionError& OutError, UHackable*& OutConnection);
};