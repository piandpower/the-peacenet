#include "UPeacenetService.h"
#include "UPeacenetServiceAsset.h"
#include "UAddressBookContext.h"
#include "UContact.h"
#include "USystemContext.h"

void UPeacenetService::NavigateToURL(FString InURL)
{
    this->EventOnNavigationRequested.Broadcast(InURL);
}

USystemContext* UPeacenetService::GetSystemContext()
{
    return this->SystemContext;
}

void UPeacenetService::Setup(USystemContext* InSystemContext, UPeacenetServiceAsset* InService)
{
    check(InSystemContext);
    check(InService);

    this->SystemContext = InSystemContext;
    this->ServiceAsset = InService;
}

void UPeacenetService::OpenPeacenetIdentity(int InEntityID, UContact*& OutContact)
{
    // Try to get the address book from our current system context.
    UAddressBookContext* AddressBook = this->GetSystemContext()->GetAddressBook();

    // Check if it was valid.
    check(AddressBook);

    // Find the relevant contact info (or add new info if the
    // contact isn't found.) We crash if the NPC isn't found in
    // the save file to begin with.
    UContact* ContactInfo = nullptr;
    check(AddressBook->GetContactByEntityID(InEntityID, ContactInfo));

    // Checks for a bug where the contact fetcher returns true but doesn't fetch a contact.
    check(ContactInfo);

    // Tell Blueprint what the FUCK we just did.
    this->EventOnContactOpenRequested.Broadcast(ContactInfo);

    // So that a Blueprint can access what we just opened.
    OutContact = ContactInfo;
}

UAddressBookContext* UPeacenetService::GetAddressBook()
{
    return this->GetSystemContext()->GetAddressBook();
}