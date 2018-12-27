#include "UUserContext.h"
#include "PeacenetWorldStateActor.h"
#include "URainbowTable.h"
#include "UAddressBookContext.h"
#include "UPeacegateFileSystem.h"

void UUserContext::Setup(USystemContext* InOwningSystem, int InUserID)
{
    // Make sure the owning system is valid.
    check(InOwningSystem);

    this->OwningSystem = InOwningSystem;
    this->UserID = InUserID;
}

FString UUserContext::GetHostname()
{
    return this->OwningSystem->GetHostname();
}

FString UUserContext::GetUsername()
{
    return this->OwningSystem->GetUsername(this->UserID);
}

FString UUserContext::GetCharacterName()
{
    return this->OwningSystem->GetCharacter().CharacterName;
}

FString UUserContext::GetHomeDirectory()
{
    return this->OwningSystem->GetUserHomeDirectory(this->UserID);
}

UPeacegateFileSystem* UUserContext::GetFilesystem()
{
    return this->OwningSystem->GetFilesystem(this->UserID);
}

URainbowTable* UUserContext::GetRainbowTable()
{
    return this->OwningSystem->GetRainbowTable();
}

UAddressBookContext* UUserContext::GetAddressBook()
{
    return this->OwningSystem->GetAddressBook();
}

APeacenetWorldStateActor* UUserContext::GetPeacenet()
{
    return this->OwningSystem->GetPeacenet();
}