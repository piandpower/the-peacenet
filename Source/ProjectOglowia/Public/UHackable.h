/********************************************************************************
 * The Peacenet - bit::phoenix("software");
 * 
 * MIT License
 *
 * Copyright (c) 2018-2019 Michael VanOverbeek, Declan Hoare, Ian Clary, 
 * Trey Smith, Richard Moch, Victor Tran and Warren Harris
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * Contributors:
 *  - Michael VanOverbeek <alkaline@bitphoenixsoftware.com>
 *
 ********************************************************************************/


#pragma once

#include "CoreMinimal.h"
#include "FComputer.h"
#include "FPeacenetIdentity.h"
#include "FFirewallRule.h"
#include "EConnectionError.h"
#include "EAuthenticationType.h"
#include "EHackCompletionType.h"
#include "UHackable.generated.h"

class UHackable;

DECLARE_DYNAMIC_DELEGATE_TwoParams(FAuthenticationRequiredEvent, EAuthenticationType, AuthType, UHackable*, Hackable);

class UComputerService;
class UPeacenetSaveGame;
class USystemContext;
class UUserContext;
class APeacenetWorldStateActor;
class ATerminalCommand;
class UPeacegateFileSystem;

UCLASS(Blueprintable, BlueprintType, Abstract)
class PROJECTOGLOWIA_API UHackable : public UObject
{
    GENERATED_BODY()

private:
    UPROPERTY()
    FString RemoteUsername;

    UPROPERTY()
    int UserID = 0;

    UPROPERTY()
    UComputerService* Service;

    UPROPERTY()
    UUserContext* OriginUserContext;

    UPROPERTY()
    USystemContext* RemoteSystem;

protected:
    UFUNCTION()
    void CompleteHack(EHackCompletionType InCompletionType);

    UFUNCTION(BlueprintImplementableEvent, Category = "Hackable")
    void HackCompleted(UUserContext* HackedUserContext);

    virtual void NativeHackCompleted(UUserContext* HackedUserContext);

    virtual void OnHackedByTerminalCommand(ATerminalCommand* InCommand) {}

public:
    UFUNCTION()
    void SetRemoteSystem(USystemContext* InSystem);

    UFUNCTION()
    void SetOriginUser(UUserContext* InOriginUser);

    UFUNCTION()
    bool SetRemoteUsername(FString InUsername);

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
    void Disconnect();

    UFUNCTION(BlueprintCallable, Category = "Networking")
    bool AuthenticateWithPrivateKeyFile(FString InPrivateKeyPath);

    UFUNCTION()
    int GetSkillLevel();

    UFUNCTION()
    void HackFromTerminalCommand(ATerminalCommand* InCommand, EHackCompletionType HackType);

public:
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Hackable")
    static FString GetConnectionErrorText(EConnectionError InError);

    UFUNCTION(BlueprintCallable, Category = "Hackable")
    static bool OpenConnection(FString InHost, int InPort, UComputerService* TargetServiceType, FString InRemoteUser, UUserContext* OriginUser, EConnectionError& OutError, UHackable*& OutConnection);
};