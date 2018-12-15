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

void UAddressBookContext::AddContact(int InEntityID, bool InIsStoryIntegral)
{
    // Info about this new contact.
    FPeacenetIdentity Character;

    // Check the system context to make sure it and The Peacenet are both valid.
    check(this->SystemContext);
    check(this->SystemContext->Peacenet);

    // Find the character by ID.
    check(this->SystemContext->Peacenet->SaveGame->GetCharacterByID(InEntityID, Character));

    // Create a pinned contact to store in the save file. This will hold most of the info that the UContact object exposes.
    FPinnedContact Contact;

    // Assign the entity IDs for loading later
    Contact.OwningEntityID = this->SystemContext->Character.ID;
    Contact.EntityID = Character.ID;

    // Make sure the contact is a person and it is/isn't story-integral depending on what the parameters to this function say.
    Contact.ContactType = EPinnedContactType::Person;
    Contact.IsStoryIntegral = InIsStoryIntegral;

    // Add to the save file and reset ourselves.
    this->SystemContext->Peacenet->SaveGame->PinnedContacts.Add(Contact);
    this->RetrieveContacts();

    // Address book's been updated.
    this->EventAddressBookUpdated.Broadcast();
}

bool UAddressBookContext::HasContact(int InEntityID)
{
    for(auto Contact : this->Contacts)
    {
        if(Contact->GetEntityID() == InEntityID)
            return true;
    }
    return false;
}

void UAddressBookContext::RemoveContactBackend(int InEntityID, bool InAllowStoryRemovals)
{
    if(this->HasContact(InEntityID))
        return;

    int ContactIndex = -1;
    for(int i = 0; i < this->SystemContext->Peacenet->SaveGame->PinnedContacts.Num(); i++)
    {
        FPinnedContact Contact = this->SystemContext->Peacenet->SaveGame->PinnedContacts[i];
        if(Contact.EntityID == InEntityID && Contact.OwningEntityID == this->SystemContext->Character.ID)
        {
            ContactIndex = i;
            break;
        }
    }

    if(ContactIndex == -1)
        return;

    // Now we can grab the contact info for this entity.
    FPinnedContact Contact = this->SystemContext->Peacenet->SaveGame->PinnedContacts[ContactIndex];

    // Enforce story checking if we're not allowed to remove story contacts.
    if(Contact.IsStoryIntegral && !InAllowStoryRemovals)
        return;

    // Remove the contact from the save file and recollect.
    this->SystemContext->Peacenet->SaveGame->PinnedContacts.RemoveAt(ContactIndex);
    this->RetrieveContacts();

    // Fire the update event.
    this->EventAddressBookUpdated.Broadcast();
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

void UAddressBookContext::RemoveContact(UContact* InContact)
{
    if(!this->Contacts.Contains(InContact))
    {
        return;
    }

    // TODO: Allow the game to choose whether the player is allowed to remove story-integral contacts. Perhaps this should be a milestone 2 feature?
    this->RemoveContactBackend(InContact->GetEntityID(), false);
}