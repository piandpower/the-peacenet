#include "UAddressBookContext.h"
#include "FContactNote.h"
#include "USystemContext.h"

void UAddressBookContext::Setup(USystemContext* InSystemContext)
{
    // Make sure the system context is valid.
    check(InSystemContext);
    check(InSystemContext->GetPeacenet());

    // Assign it to us.
    this->SystemContext = InSystemContext;

    // Load in the initial contact list.
    this->RetrieveContacts();
}

void UAddressBookContext::AddContact(int InEntityID, bool InIsStoryIntegral)
{
    // Info about this new contact.
    FPeacenetIdentity Character;
    int CharacterIndex;

    // Check the system context to make sure it and The Peacenet are both valid.
    check(this->SystemContext);
    check(this->SystemContext->GetPeacenet());

    // Find the character by ID.
    check(this->SystemContext->GetPeacenet()->SaveGame->GetCharacterByID(InEntityID, Character, CharacterIndex));

    // Create a pinned contact to store in the save file. This will hold most of the info that the UContact object exposes.
    FPinnedContact Contact;

    // Assign the entity IDs for loading later
    Contact.OwningEntityID = this->SystemContext->GetCharacter().ID;
    Contact.EntityID = Character.ID;

    // Make sure the contact is a person and it is/isn't story-integral depending on what the parameters to this function say.
    Contact.ContactType = EPinnedContactType::Person;
    Contact.IsStoryIntegral = InIsStoryIntegral;

    // Add to the save file and reset ourselves.
    this->SystemContext->GetPeacenet()->SaveGame->PinnedContacts.Add(Contact);
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
    if(!this->HasContact(InEntityID))
        return;

    int ContactIndex = -1;
    for(int i = 0; i < this->SystemContext->GetPeacenet()->SaveGame->PinnedContacts.Num(); i++)
    {
        FPinnedContact Contact = this->SystemContext->GetPeacenet()->SaveGame->PinnedContacts[i];
        if(Contact.EntityID == InEntityID && Contact.OwningEntityID == this->SystemContext->GetCharacter().ID)
        {
            ContactIndex = i;
            break;
        }
    }

    if(ContactIndex == -1)
        return;

    // Now we can grab the contact info for this entity.
    FPinnedContact Contact = this->SystemContext->GetPeacenet()->SaveGame->PinnedContacts[ContactIndex];

    // Enforce story checking if we're not allowed to remove story contacts.
    if(Contact.IsStoryIntegral && !InAllowStoryRemovals)
        return;

    // Remove the contact from the save file and recollect.
    this->SystemContext->GetPeacenet()->SaveGame->PinnedContacts.RemoveAt(ContactIndex);
    this->RetrieveContacts();

    // Fire the update event.
    this->EventAddressBookUpdated.Broadcast();
}

void UAddressBookContext::RetrieveContacts()
{
    // Clear the current contact list.
    this->Contacts.Empty();

    // Go through every contact in the save file.
    for(int i = 0; i < this->SystemContext->GetPeacenet()->SaveGame->PinnedContacts.Num(); i++)
    {
        FPinnedContact PinnedContact = this->SystemContext->GetPeacenet()->SaveGame->PinnedContacts[i];

        // Is the contact not owned by us? If it isn't owned by us we skip it.
        if(PinnedContact.OwningEntityID != this->SystemContext->GetCharacter().ID)
            continue;

        // Create a new Contact object.
        UContact* Contact = NewObject<UContact>(this);

        // Initialize it so it's owned by us and represents this pinned contact entity.
        Contact->Setup(this, PinnedContact.EntityID, i);

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

bool UAddressBookContext::FindCharacterByID(int InEntityID, FPeacenetIdentity& OutCharacter, int& OutIndex)
{
    return this->SystemContext->GetPeacenet()->SaveGame->GetCharacterByID(InEntityID, OutCharacter, OutIndex);
}

bool UAddressBookContext::FindComputerAndContact(FPeacenetIdentity InCharacter, FComputer& OutComputer, FPinnedContact& OutContact, int& OutComputerIndex)
{
    if(!this->SystemContext->GetPeacenet()->SaveGame->GetComputerByID(InCharacter.ComputerID, OutComputer, OutComputerIndex))
        return false;

    for(int i = 0; i < this->SystemContext->GetPeacenet()->SaveGame->PinnedContacts.Num(); i++)
    {
        FPinnedContact Contact = this->SystemContext->GetPeacenet()->SaveGame->PinnedContacts[i];

        if(Contact.EntityID == InCharacter.ID && Contact.OwningEntityID == this->SystemContext->GetCharacter().ID)
        {
            OutContact = Contact;
            return true;
        }
    }
    return false;
}

bool UAddressBookContext::GetContactByEntityID(int InEntityID, UContact*& OutContact)
{
    for(auto Contact : this->Contacts)
    {
        if(Contact->GetEntityID() == InEntityID)
        {
            OutContact = Contact;
            return true;
        }
    }

    // We'll add a new contact if none was found and the entity exists.
    FPeacenetIdentity Character;
    int CharacterIndex;
    if(this->SystemContext->GetPeacenet()->SaveGame->GetCharacterByID(InEntityID, Character, CharacterIndex))
    {
        FPinnedContact NewContact;
        NewContact.EntityID = InEntityID;
        NewContact.OwningEntityID = this->SystemContext->GetCharacter().ID;
        
        NewContact.IsStoryIntegral = Character.CharacterType == EIdentityType::Story;

        NewContact.ContactType = EPinnedContactType::Person;
        this->SystemContext->GetPeacenet()->SaveGame->PinnedContacts.Add(NewContact);

        // MANUALLY create the UContact context.
        UContact* ContactContext = NewObject<UContact>(this);
        ContactContext->Setup(this, NewContact.EntityID, this->SystemContext->GetPeacenet()->SaveGame->PinnedContacts.Num() - 1);

        // Add it to our contact list.
        this->Contacts.Add(ContactContext);

        OutContact = ContactContext;

        this->EventAddressBookUpdated.Broadcast();

        return true;
    }

    return false;
}

void UAddressBookContext::DiscoverName(UContact* InContact)
{
    this->SystemContext->GetPeacenet()->SaveGame->PinnedContacts[InContact->GetContactIndex()].IsNameKnown = true;
}

void UAddressBookContext::DiscoverEmail(UContact* InContact)
{
    this->SystemContext->GetPeacenet()->SaveGame->PinnedContacts[InContact->GetContactIndex()].IsEmailKnown = true;
}

void UAddressBookContext::DiscoverCountry(UContact* InContact)
{
    this->SystemContext->GetPeacenet()->SaveGame->PinnedContacts[InContact->GetContactIndex()].IsCountryKnown = true;
}

void UAddressBookContext::DiscoverReputation(UContact* InContact)
{
    this->SystemContext->GetPeacenet()->SaveGame->PinnedContacts[InContact->GetContactIndex()].IsReputationKnown = true;
}

void UAddressBookContext::DiscoverIPAddress(UContact* InContact)
{
    this->SystemContext->GetPeacenet()->SaveGame->PinnedContacts[InContact->GetContactIndex()].IsPersonalIPKnown = true;
}

void UAddressBookContext::AddUserNote(UContact* InContact, const FText& InUserNote)
{
    FPinnedContact Contact = this->SystemContext->GetPeacenet()->SaveGame->PinnedContacts[InContact->GetContactIndex()];

    FContactNote NewNote;
    NewNote.NoteMessage = InUserNote;
    NewNote.Timestamp = this->SystemContext->GetPeacenet()->SaveGame->EpochTime;

    Contact.UserNotes.Add(NewNote);

    this->SystemContext->GetPeacenet()->SaveGame->PinnedContacts[InContact->GetContactIndex()] = Contact;
}