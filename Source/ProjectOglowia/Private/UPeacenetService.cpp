#include "UPeacenetService.h"
#include "UPeacenetServiceAsset.h"
#include "USystemContext.h"

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