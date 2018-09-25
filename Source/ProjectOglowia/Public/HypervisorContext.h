// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "FComputer.h"
#include "FPeacenetIdentity.h"
#include "UObject/Interface.h"
#include "HypervisorContext.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHypervisorContext : public UInterface
{
	GENERATED_BODY()
};

/**
 * An interface containing functionality for a Peacenet hypervisor context object.
 */
class PROJECTOGLOWIA_API IHypervisorContext
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hypervisor Context")
	bool FindComputerByHostname(const FText& InHostname, FComputer& OutComputer);
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hypervisor Context")
	bool FindComputerByEntityID(int InEntityID, FComputer& OutComputer);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hypervisor Context")
	void UpdateComputerByEntityID(int InEntityID, const FComputer InComputer);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hypervisor Context")
	void UpdateCharacterByEntityID(int InEntityID, const FPeacenetIdentity InCharacterIdentity);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hypervisor Context")
	bool FindCharacterByEntityID(int InEntityID, FPeacenetIdentity& OutComputer);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hypervisor Context")
	FPeacenetIdentity FindOrCreateSinglePlayerCharacter();
};
