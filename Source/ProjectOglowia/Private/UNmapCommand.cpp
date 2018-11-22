// Copyright (c) 2018 The Peacenet & Alkaline Thunder.


#include "UNmapCommand.h"
#include "UComputerTypeAsset.h"
#include "TimerManager.h"
#include "FNetMapScanEventArgs.h"
#include "UHackableAsset.h"

void UNmapCommand::ListNextService()
{
	if (this->Hackables.Num())
	{
		UHackableAsset* Hackable = this->Hackables[0];
		this->Hackables.RemoveAt(0);
		this->Caller->Write(Hackable->InternalID.ToString() + "\t");
		this->Caller->Write("open\t");
		this->Caller->WriteLine(FString::FromInt(Hackable->Port));

		FNetMapScanEventArgs EventArgs;
		EventArgs.Hackable = Hackable;
		EventArgs.EventType = ENetMapScanEventType::ServiceFound;

		this->Caller->SystemContext->BroadcastNetMapEvent(this->ResolvedContext->Computer.ID, EventArgs);

		FTimerHandle UnusedHandle;
		this->Caller->SystemContext->Peacenet->GetWorldTimerManager().SetTimer(
			UnusedHandle, this, &UNmapCommand::ListNextService, 0.6f, false);
	}
	else
	{
		this->Complete();
	}
}

void UNmapCommand::RunCommand(UConsoleContext* InConsole, const TMap<FString, UDocoptValue*> InArguments)
{
    InConsole->WriteLine("Starting Nmap...");
    
    FString UserIP = InArguments["<host>"]->AsString();

    FString ResolvedIP;

    if(InConsole->SystemContext->Peacenet->ResolveHost(UserIP, ResolvedIP, ResolvedContext))
    {
        InConsole->WriteLine("Nmap scan report for " + UserIP + " (" + ResolvedIP + ")");
        InConsole->WriteLine("Host is up.");
        InConsole->WriteLine("");
        InConsole->WriteLine("PORT\tSTATE\tSERVICE");

        for(auto ComputerTypeAsset : InConsole->SystemContext->Peacenet->ComputerTypes)
        {
            if(ComputerTypeAsset->InternalID == ResolvedContext->Computer.ComputerType)
            {
				this->Caller = InConsole;
				this->Hackables = ComputerTypeAsset->Services;

				FNetMapScanEventArgs EventArgs;
				EventArgs.EventType = ENetMapScanEventType::ScanStarted;

				this->Caller->SystemContext->BroadcastNetMapEvent(this->ResolvedContext->Computer.ID, EventArgs);

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