// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "UVirtualMachineService.generated.h"

class AVirtualMachine; //not going through that headache again, not #including it here.
class UVirtualMachineService; //We use this...before...it's...declared...fuck me in the ass.

USTRUCT(BlueprintType)
struct FServiceInfo
{
	GENERATED_BODY()

	// The port used to communicate with this service over the Peacenet hypervisor.
	UPROPERTY(BlueprintReadWrite)
	int Port;

	// The name of this service in manual pages and in the UI.
	UPROPERTY(BlueprintReadWrite)
	FString Name;
	
	// The description of this service in manual pages.
	UPROPERTY(BlueprintReadWrite)
	FString Description;

	//The Virtual Machine Service class responsible for running this Peacegate Service.
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UVirtualMachineService> ServiceClass;
};

UCLASS(Blueprintable)
class UServiceInfoAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Peacegate Service Asset")
		FServiceInfo Info;

	// The name of this service used internally by the game's code to identify it.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Peacegate Service Asset")
		FName InternalServiceName;
};

UCLASS(Blueprintable)
class UServiceMapAsset : public UDataAsset 
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName MachineType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UServiceInfoAsset*> SupportedServices;
};



/**
 * A hackable system service that runs within Peacegate OS.
 */
UCLASS()
class PROJECTOGLOWIA_API UVirtualMachineService : public UObject
{
	GENERATED_BODY()

public:
	UVirtualMachineService() {}

	// The machine that runs and operates this System Service.
	UPROPERTY(BlueprintReadOnly, Category="Peacegate System Service")
	AVirtualMachine* OwnerMachine;

	// Information about this service, such as its name, description, port, etc.
	UPROPERTY(BlueprintReadOnly, Category = "Peacegate System Service")
	FServiceInfo ServiceInfo;
	
	void LoadService(FServiceInfo& InServiceInfo, AVirtualMachine* InVirtualMachine);

protected:
	virtual void NativeServiceLoad() {}
	virtual void NativeTickService(float DeltaTime) {}

public:
	void OnTickService(float DeltaTime);
	void OnServiceLoad();


	UFUNCTION(BlueprintImplementableEvent, Category="Peacegate System Service")
	void ServiceLoad();

	UFUNCTION(BlueprintImplementableEvent, Category = "Peacegate System Service")
	void TickService(float DeltaTime);
};
