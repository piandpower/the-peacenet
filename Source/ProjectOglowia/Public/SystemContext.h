// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VirtualMachine.h"
#include "UCommandProcessor.h"
#include "SystemContext.generated.h"


USTRUCT(BlueprintType)
struct FUserInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Username;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool IsAdminUser = false;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USystemContext : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTOGLOWIA_API ISystemContext
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "System Context")
	FString GetHostname();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "System Context")
	float GetGovernmentAlertLevelPercentage();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "System Context")
	ECountry GetCurrentCountry();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "System Context")
	float GetReputation();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "System Context")
	FUserInfo GetUserInfo(const int InUserID);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "System Context")
	void ReportCommandRun(const FCommandRunInstruction InCommand);
};
