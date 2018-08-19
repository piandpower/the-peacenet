// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UVirtualMachineService.h"
#include "VirtualMachine.h"

void UVirtualMachineService::LoadService(FServiceInfo & InServiceInfo, AVirtualMachine * InVirtualMachine)
{
	ServiceInfo = InServiceInfo;
	OwnerMachine = InVirtualMachine;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Loaded system service \"") + ServiceInfo.Name + TEXT("\" on ") + OwnerMachine->Hostname + TEXT(":") + FString::FromInt(ServiceInfo.Port) + TEXT("."));

	OnServiceLoad();
}

void UVirtualMachineService::OnServiceLoad()
{
	NativeServiceLoad();
	ServiceLoad();
}

void UVirtualMachineService::OnTickService(float DeltaTime)
{
	NativeTickService(DeltaTime);
	TickService(DeltaTime);
}
