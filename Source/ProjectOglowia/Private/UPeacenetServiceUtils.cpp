#include "UPeacenetServiceUtils.h"
#include "Blueprint/UserWidget.h"
#include "UProgram.h"
#include "UWindow.h"
#include "UUserContext.h"
#include "USystemContext.h"
#include "UPeacenetService.h"
#include "AssetRegistryModule.h"

bool UPeacenetServiceUtils::GetService(FString InServiceURL, UPeacenetServiceAsset*& OutService)
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

    TArray<FAssetData> WebsiteAssets;
    check(AssetRegistryModule.Get().GetAssetsByClass("PeacenetServiceAsset", WebsiteAssets));

    for(auto& Asset : WebsiteAssets)
    {
        UPeacenetServiceAsset* Website = Cast<UPeacenetServiceAsset>(Asset.GetAsset());
        if(Website->ServiceURL == InServiceURL)
        {
            OutService = Website;
            return true;
        }
    }

    return false;
}

UPeacenetService* UPeacenetServiceUtils::CreateServiceWidget(UProgram* InOwningProgram, UPeacenetServiceAsset* InService)
{
    UPeacenetService* Service = CreateWidget<UPeacenetService>(InOwningProgram->GetOwningPlayer(), InService->ServiceWidget);

    Service->Setup(InOwningProgram->GetUserContext()->GetOwningSystem(), InService);

    return Service;
}