#include "UContact.h"
#include "UAddressBookContext.h"

void UContact::Setup(UAddressBookContext* InOwnerAddressBook, int InEntityID)
{
    check(InOwnerAddressBook);

    this->AddressBook = InOwnerAddressBook;
    this->EntityID = InEntityID;
}

int UContact::GetEntityID()
{
    return this->EntityID;
}

bool UContact::FetchCharacter(FPeacenetIdentity& OutCharacter, FComputer& OutComputer, FPinnedContact& OutContact)
{
    if(!this->AddressBook->FindCharacterByID(this->EntityID, OutCharacter))
        return false;

    return this->AddressBook->FindComputerAndContact(OutCharacter, OutComputer, OutContact);
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
    // Not yet implemented.
    return false; 
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
