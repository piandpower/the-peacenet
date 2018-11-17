// Copyright (c) 2018 The Peacenet & Alkaline Thunder.


#include "UNmapCommand.h"
#include "UComputerTypeAsset.h"
#include "UHackableAsset.h"

void UNmapCommand::RunCommand(UConsoleContext* InConsole, const TMap<FString, UDocoptValue*> InArguments) 
{
    InConsole->WriteLine("Starting Nmap...");
    
    FString UserIP = InArguments["<host>"]->AsString();

    USystemContext* ResolvedContext;
    FString ResolvedIP;

    if(InConsole->SystemContext->Peacenet->ResolveHost(UserIP, ResolvedIP, ResolvedContext))
    {
        InConsole->WriteLine("Nmap scan report for " + UserIP + " (" + ResolvedIP + ")");
        InConsole->WriteLine("Host is up.");
        InConsole->WriteLine("");
        InConsole->WriteLine("PORT\tSTATE\tSERVICE");

        UComputerTypeAsset* ComputerType;
        for(auto ComputerTypeAsset : InConsole->SystemContext->Peacenet->ComputerTypes)
        {
            if(ComputerTypeAsset->InternalID == ResolvedContext->Computer.ComputerType)
            {
                for(auto Hackable : ComputerTypeAsset->Services)
                {
                    InConsole->Write(FString::FromInt(Hackable->Port)+"\t");
                    InConsole->Write("open\t");
                    InConsole->WriteLine(Hackable->InternalID.ToString());
                }
            }
        }
    }
    else
    {
        InConsole->WriteLine("Failed to resolve \"" + UserIP + "\".");
    }
    this->Complete();
}