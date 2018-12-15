#include "UContact.h"
#include "UAddressBookContext.h"

void UContact::Setup(UAddressBookContext* InOwnerAddressBook, int InEntityID)
{
    check(InOwnerAddressBook);

    this->AddressBook = InOwnerAddressBook;
    this->EntityID = InEntityID;
}