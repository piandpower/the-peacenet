#include "UUserContext.h"

void UUserContext::Setup(USystemContext* InOwningSystem, int InUserID)
{
    // Make sure the owning system is valid.
    check(InOwningSystem);

    this->OwningSystem = InOwningSystem;
    this->UserID = InUserID;
}