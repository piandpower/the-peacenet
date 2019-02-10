#include "UHackable.h"
#include "USystemContext.h"
#include "UUserContext.h"
#include "UPeacegateFileSystem.h"
#include "PeacenetWorldStateActor.h"
#include "UComputerService.h"
#include "UPeacenetSaveGame.h"

void UHackable::NativeHackCompleted()
{

}

void UHackable::CompleteHack()
{
    this->NativeHackCompleted();
    this->HackCompleted();
}

void UHackable::StartAuth(FAuthenticationRequiredEvent InCallback)
{
    if(this->NeedsAuthentication())
    {
        InCallback.Execute(this->GetAuthenticationType(), this);
    }
    else
    {
        this->CompleteHack();
    }
}

bool UHackable::NeedsAuthentication()
{
    return this->GetAuthenticationType() == EAuthenticationType::None;
}

bool UHackable::AuthenticateWithPassword(FString InPassword)
{
    if(this->GetAuthenticationType() != EAuthenticationType::Basic)
        return false;

    // todo: fuck me
    return true;
}
    
bool UHackable::AuthenticateWithUsernameAndPassword(FString InUsername, FString InPassword)
{
    if(this->GetAuthenticationType() != EAuthenticationType::Credential)
        return false;

    // todo: fuck me
    return true;
}

bool UHackable::AuthenticateWithPrivateKeyFile(FString InPrivateKeyPath)
{
    if(this->GetAuthenticationType() != EAuthenticationType::Crypto)
        return false;

    // todo: fuck me
    return true;
}

EAuthenticationType UHackable::GetAuthenticationType()
{
    return this->Service->AuthenticationType;
}

bool UHackable::OpenConnection(FString InHost, int InPort, UComputerService* TargetServiceType, UUserContext* OriginUser, EConnectionError& OutError, UHackable*& OutConnection)
{
    // The origin User Context has a reference to Peacenet, so we better not get nullptr.
    check(OriginUser);
    check(OriginUser->GetOwningSystem());
    check(OriginUser->GetPeacenet());

    // We're gonna need this a lot.
    auto Peacenet = OriginUser->GetPeacenet();

    // So here's how this is going to work.
    //
    // First we need to resolve the hostname provided by the caller.
    // Like in C#'s networking libraries, Peacenet has a function to do that.
    // If the host is in fact a HOSTNAME and not an IP, the game will try
    // to first resolve it to an IP. If that fails, it'll send us a "HostNotFound"
    // connection error.
    //
    // Once it has an IP address, it will try to resolve that to an entity ID.
    // If it can do that, it will send us a reference to the computer it found.
    // If not, we'll get a "ConnectionTimedOut" error.
    
    // So, we need an FComputer variable here for later...
    FComputer RemoteComputer;

    // C++ is nice.
    if(!Peacenet->ResolveHost(InHost, RemoteComputer, OutError))
        return false;
    
    // If we have gotten this far, we have ALMOST everything we need to 
    // open a connection to the remote system.
    //
    // Because this is a game, we need to do a few checks - we can't let
    // the player connect if there's a firewall filter!
    //
    // We also can't connect if the intended service isn't running on
    // the specified port.

    for(auto FirewallRule : RemoteComputer.FirewallRules)
    {
        // Does the port match?
        if(FirewallRule.Port == InPort)
        {
            // If it's filtered, refuse to connect.
            if(FirewallRule.IsFiltered)
            {
                OutError = EConnectionError::ConnectionRefused;
                return false;
            }

            // If the service running on this port doesn't match the class of the intended service, we refuse to connect.
            if(!(FirewallRule.Service && FirewallRule.Service == TargetServiceType))
            {
                OutError = EConnectionError::ConnectionRefused;
                return false;
            }

            // We can now open a connection.
            OutConnection = NewObject<UHackable>(OriginUser, FirewallRule.Service->Hackable);

            // Link the origin user to the hackable context.
            OutConnection->SetOriginUser(OriginUser);

            // Now we need to somehow link the connection to a remote system context.
            // Since we have an FComputer, we have an entity ID, so we can do that.
            // 
            // The issue is with characters...
            //
            // We need an Identity ID as well to initialize the system context.
            int Identity;
            bool result = Peacenet->GetOwningIdentity(RemoteComputer, Identity);
            check(result);

            // The debugger should hault us if we couldn't get an identity ID. But if it's a shipping build we'll just timeout.
            if(!result)
            {
                OutError = EConnectionError::ConnectionTimedOut;
                OutConnection = nullptr;
                return false;
            }

            // The connection may need to know what kind of connection it is.
            OutConnection->SetService(FirewallRule.Service);

            // Grab a remote system context.
            USystemContext* RemoteContext = Peacenet->GetSystemContext(Identity);

            // Should be fully set up.
            OutConnection->SetRemoteSystem(RemoteContext);

            // All done!
            OutError = EConnectionError::None;
            return true;
        }
    }

    OutError = EConnectionError::ConnectionRefused;
    return false;
}

FString UHackable::GetConnectionErrorText(EConnectionError InError)
{
    switch(InError)
    {
        case EConnectionError::HostNotFound:
            return "Could not resolve hostname. No such host is known.";
        case EConnectionError::ConnectionTimedOut:
            return "Connection timed out.";
        case EConnectionError::ConnectionRefused:
            return "Connection refused.";
    }
    return "Unknown error.";
}

void UHackable::SetOriginUser(UUserContext* InOriginUser)
{
    check(InOriginUser);
    check(!this->OriginUserContext);

    this->OriginUserContext = InOriginUser;
}

void UHackable::SetRemoteSystem(USystemContext* InSystem)
{
    check(this->OriginUserContext);
    check(InSystem);
    check(!this->RemoteSystem);

    this->RemoteSystem = InSystem;

    // Add ourselves as an outbound connection from the origin system.
    // That way the origin can close it at will.
    this->OriginUserContext->GetOwningSystem()->AddConnection(this, false);

    // Add ourselves as an INBOUND connection to the target system.
    // This allows the target system to know something's connecting to it.
    // It also means that both systems mutually own the connection, which
    // means mutually-assured anti-garbage-collection.
    //
    // If the target is the player, their desktop will screech at them too.
    this->RemoteSystem->AddConnection(this, true);
}

void UHackable::SetService(UComputerService* InService)
{
    check(!this->Service);
    check(InService);

    this->Service = InService;
}