// Copyright (c) 2018 The Peacenet & Alkaline Thunder.


#include "UNmapCommand.h"
#include "UComputerTypeAsset.h"
#include "TimerManager.h"
#include "UHackableAsset.h"

void UNmapCommand::ListNextService()
{
	if (this->Hackables.Num())
	{
		FServiceInfo Hackable = this->Hackables[0];
		this->Hackables.RemoveAt(0);
		UComputerService* Service = nullptr;
		if (this->ResolvedContext->GetPeacenet()->FindServiceByName(Hackable.ServiceName, Service))
		{
			this->Caller->Write(Hackable.ServiceName.ToString() + "\t");
			this->Caller->Write("open\t");
			this->Caller->WriteLine(FString::FromInt(Hackable.Port));

			FNetMapScanEventArgs EventArgs;
			FNetMapServiceInfo NetMapService;
			NetMapService.ServiceAsset = Service;
			NetMapService.ServiceInfo = Hackable;
			EventArgs.Hackable = NetMapService;
			EventArgs.EventType = ENetMapScanEventType::ServiceFound;

			this->NetMapEvent.Broadcast(this->ResolvedContext->GetComputer().ID, EventArgs);
		}
		FTimerHandle UnusedHandle;
		this->Caller->SystemContext->GetPeacenet()->GetWorldTimerManager().SetTimer(
			UnusedHandle, this, &UNmapCommand::ListNextService, 0.6f, false);
	}
	else
	{
		this->Complete();
	}
}

void UNmapCommand::NativeRunCommand(UConsoleContext* InConsole, const TMap<FString, UDocoptValue*> InArguments)
{
    InConsole->WriteLine("Starting Nmap...");
    
    FString UserIP = InArguments["<host>"]->AsString();

    FString ResolvedIP;

    if(InConsole->SystemContext->GetPeacenet()->ResolveHost(UserIP, ResolvedIP, ResolvedContext))
    {
        InConsole->WriteLine("Nmap scan report for " + UserIP + " (" + ResolvedIP + ")");
        InConsole->WriteLine("Host is up.");
        InConsole->WriteLine("");
        InConsole->WriteLine("PORT\tSTATE\tSERVICE");

        for(auto ComputerTypeAsset : InConsole->SystemContext->GetPeacenet()->ComputerTypes)
        {
            if(ComputerTypeAsset->InternalID == ResolvedContext->GetComputer().ComputerType)
            {
				this->Caller = InConsole;
				this->Hackables.Empty();

				for (auto Service : ResolvedContext->GetComputer().ActiveServices)
				{
					this->Hackables.Add(Service);
				}

				FNetMapScanEventArgs EventArgs;
				EventArgs.EventType = ENetMapScanEventType::ScanStarted;

				this->NetMapEvent.Broadcast(this->ResolvedContext->GetComputer().ID, EventArgs);

				this->ListNextService();
			}
        }
    }
    else
    {
        InConsole->WriteLine("Failed to resolve \"" + UserIP + "\".");
		this->Complete();
	}
}