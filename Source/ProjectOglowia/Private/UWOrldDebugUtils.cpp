// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UWOrldDebugUtils.h"
#include "USystemContext.h"
#include "UProgram.h"
#include "UWindow.h"
#include "UPeacenetService.h"
#include "UConsoleContext.h"
#include "PeacenetWorldStateActor.h"
#include "UPeacenetSaveGame.h"
#include "TerminalCommand.h"
#include "UUserContext.h"
#include "UDesktopWidget.h"

bool UWorldDebugUtils::RetrieveSystemContext(UObject* InContextObject, USystemContext*& OutSystemContext)
{
    if(Cast<USystemContext>(InContextObject))
    {
        OutSystemContext = Cast<USystemContext>(InContextObject);
        return true;
    }

    if(Cast<UDesktopWidget>(InContextObject))
    {
        OutSystemContext = Cast<UDesktopWidget>(InContextObject)->SystemContext;
        return true;
    }

    if(Cast<UTerminalCommand>(InContextObject))
    {
        OutSystemContext = Cast<UTerminalCommand>(InContextObject)->GetConsole()->GetUserContext()->GetOwningSystem();
        return true;
    }

    if(Cast<UWindow>(InContextObject))
    {
        OutSystemContext = Cast<UWindow>(InContextObject)->GetUserContext()->GetOwningSystem();
        return true;
    }

    if(Cast<UProgram>(InContextObject))
    {
        OutSystemContext = Cast<UProgram>(InContextObject)->GetUserContext()->GetOwningSystem();
        return true;
    }

    if(Cast<UConsoleContext>(InContextObject))
    {
        OutSystemContext = Cast<UConsoleContext>(InContextObject)->GetUserContext()->GetOwningSystem();
        return true;
    }

    if(Cast<UPeacenetService>(InContextObject))
    {
        OutSystemContext = Cast<UPeacenetService>(InContextObject)->GetSystemContext();
        return true;
    }

    return false;
}

bool UWorldDebugUtils::GetNPCs(UObject* InContextObject, TArray<FPeacenetIdentity>& OutCharacters)
{
    USystemContext* SystemContext = nullptr;
    if(!RetrieveSystemContext(InContextObject, SystemContext))
        return false;
    
    OutCharacters = SystemContext->GetPeacenet()->SaveGame->Characters;
    return true;
}

bool UWorldDebugUtils::GetNPCComputer(UObject* InContextObject, const FPeacenetIdentity& InCharacter, FComputer& OutComputer)
{
    USystemContext* SystemContext = nullptr;
    if(!RetrieveSystemContext(InContextObject, SystemContext))
        return false;

    int ComputerIndex;

    return SystemContext->GetPeacenet()->SaveGame->GetComputerByID(InCharacter.ComputerID, OutComputer, ComputerIndex);
}