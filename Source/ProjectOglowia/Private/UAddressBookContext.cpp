#include "UAddressBookContext.h"
#include "USystemContext.h"

void UAddressBookContext::Setup(USystemContext* InSystemContext)
{
    // Make sure the system context is valid.
    check(InSystemContext);
    check(InSystemContext->Peacenet);

    // Assign it to us.
    this->SystemContext = InSystemContext;

    // Load in the initial contact list.
    this->RetrieveContacts();
}

void UAddressBookContext::RetrieveContacts()
{
    // Clear the current contact list.
    this->Contacts.Empty();

    // Go through every contact in the save file.
    for(FPinnedContact& PinnedContact : this->SystemContext->Peacenet->SaveGame->PinnedContacts)
    {
        // Is the contact not owned by us? If it isn't owned by us we skip it.
        if(PinnedContact.OwningEntityID != this->SystemContext->Character.ID)
            continue;

        // Create a new Contact object.
        UContact* Contact = NewObject<UContact>(this);

        // Initialize it so it's owned by us and represents this pinned contact entity.
        Contact->Setup(this, PinnedContact.EntityID);

        // Add to our Contacts list!
        this->Contacts.Add(Contact);
    }
}

TArray<UContact*> UAddressBookContext::GetContacts()
{
    return this->Contacts;
}