#include "UContact.h"
#include "UAddressBookContext.h"

void UContact::Setup(UAddressBookContext* InOwnerAddressBook, int InEntityID, int InContactIndex)
{
    check(InOwnerAddressBook);

    this->AddressBook = InOwnerAddressBook;
    this->ContactIndex = InContactIndex;
    this->EntityID = InEntityID;
}

bool UContact::GetUserNotes(TArray<FContactNote>& OutNotes)
{
    FPeacenetIdentity Character;
    FComputer Computer;
    FPinnedContact Contact;
    if(!FetchCharacter(Character, Computer, Contact))
        return false;

    OutNotes = Contact.UserNotes;
    return OutNotes.Num();
}

int UContact::GetContactIndex()
{
    return this->ContactIndex;
}

int UContact::GetEntityID()
{
    return this->EntityID;
}

bool UContact::FetchCharacter(FPeacenetIdentity& OutCharacter, FComputer& OutComputer, FPinnedContact& OutContact)
{
    int CharacterIndex;
    int ComputerIndex;

    if(!this->AddressBook->FindCharacterByID(this->EntityID, OutCharacter, CharacterIndex))
        return false;

    return this->AddressBook->FindComputerAndContact(OutCharacter, OutComputer, OutContact, ComputerIndex);
}

bool UContact::RetrieveName(FString& OutCharacterName)
{
    FPeacenetIdentity Character;
    FComputer Computer;
    FPinnedContact Contact;
    if(!FetchCharacter(Character, Computer, Contact))
        return false;

    if(!Contact.IsNameKnown)
        return false;

    OutCharacterName = Character.CharacterName;
    return true;
}

bool UContact::RetrieveEmail(FString& OutEmail)
{
    FPeacenetIdentity Character;
    FComputer Computer;
    FPinnedContact Contact;
    if(!FetchCharacter(Character, Computer, Contact))
        return false;

    if(!Contact.IsEmailKnown)
        return false;

    OutEmail = Character.Email;
    return true;    // Not yet implemented.
}

bool UContact::RetrieveCountry(ECountry& OutCountry)
{
    FPeacenetIdentity Character;
    FComputer Computer;
    FPinnedContact Contact;
    if(!FetchCharacter(Character, Computer, Contact))
        return false;

    if(!Contact.IsCountryKnown)
        return false;

    OutCountry = Character.Country;
    return true;

}

bool UContact::RetrieveReputation(float& OutReputation)
{
    FPeacenetIdentity Character;
    FComputer Computer;
    FPinnedContact Contact;
    if(!FetchCharacter(Character, Computer, Contact))
        return false;

    if(!Contact.IsReputationKnown)
        return false;


    OutReputation = Character.Reputation;
    return true;

}

bool UContact::RetrieveComputerIP(FString& OutIPAddress)
{
    FPeacenetIdentity Character;
    FComputer Computer;
    FPinnedContact Contact;
    if(!FetchCharacter(Character, Computer, Contact))
        return false;

    if(!Contact.IsPersonalIPKnown)
        return false;

    OutIPAddress = Computer.IPAddress;
    return true;

}

bool UContact::RetrieveComputerHostname(FString& OutHostname)
{
    FPeacenetIdentity Character;
    FComputer Computer;
    FPinnedContact Contact;
    if(!FetchCharacter(Character, Computer, Contact))
        return false;



    OutHostname = "unknown";

    return true;
}
