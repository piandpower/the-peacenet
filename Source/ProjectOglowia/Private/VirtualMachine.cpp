// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "VirtualMachine.h"


// Sets default values
AVirtualMachine::AVirtualMachine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AVirtualMachine::SpinUpSystem(const FMachineInfo & InMachineInfo)
{
	Hostname = InMachineInfo.Hostname;

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> AssetData;
	AssetRegistryModule.Get().GetAssetsByClass(FName(TEXT("ServiceMapAsset")), AssetData, true);

	for (auto& data : AssetData)
	{
		UServiceMapAsset* mapAsset = Cast<UServiceMapAsset>(data.GetAsset());

		if (mapAsset->MachineType == InMachineInfo.MachineType)
		{
			for (auto& serviceAsset : mapAsset->SupportedServices)
			{
				UClass* serviceClass = serviceAsset->Info.ServiceClass;
				
				auto service = NewObject<UVirtualMachineService>(this, serviceClass);
				service->LoadService(serviceAsset->Info, this);
				Services.Add(service);
			}
		}
	}
}

// Called when the game starts or when spawned
void AVirtualMachine::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVirtualMachine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Tick all the services.
	for (auto& service : Services)
	{
		service->OnTickService(DeltaTime);
	}
}

