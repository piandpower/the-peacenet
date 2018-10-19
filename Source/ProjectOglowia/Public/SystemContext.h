// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ECountry.h"
#include "UCommandProcessor.h"
#include "WallpaperAsset.h"
#include "FComputer.h"
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
UINTERFACE(MinimalAPI, Blueprintable)
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
	FString GetHostname() const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "System Context")
	UPeacegateFileSystem* GetFilesystem(const int UserID);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = " System Context")
	TScriptInterface<ICommandSeeker> GetCommandSeeker();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "System Context")
	FUserInfo GetUserInfo(const int InUserID);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "System Context")
	void ShowWindowOnWorkspace(const UProgram* InProgram);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "System Context")
	EUserDomain GetUserDomain(int InUserID);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "System Context")
	FText GetUsername(int InUserID);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "System Context")
	FString GetUserHomeDirectory(int UserID);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hacking")
	bool Authenticate(const FString& Username, const FString& Password, int& UserID);

	UFUNCTION(BlueprintImplementableEvent, Category = "Peacegate")
	bool GetSuitableProgramForFileExtension(const FString& InExtension, class UPeacegateProgramAsset*& OutProgram);
};
