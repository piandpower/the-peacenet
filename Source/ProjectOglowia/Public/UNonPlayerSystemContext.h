// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "SystemContext.h"
#include "FolderRepository.h"
#include "UNonPlayerSystemContext.generated.h"

/**
 * A system context for an NPC.
 */
UCLASS(BlueprintType)
class PROJECTOGLOWIA_API UNonPlayerSystemContext : public UObject, public ISystemContext, public IFolderRepository
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Meta=(ExposeOnSpawn))
	FComputer Computer;

public:
	/*** ISystemContext ***/

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "System Context")
	FString GetHostname() const;
	virtual FString GetHostname_Implementation() const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "System Context")
	UPeacegateFileSystem* GetFilesystem(const int UserID);
	virtual UPeacegateFileSystem* GetFilesystem_Implementation(const int UserID);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = " System Context")
	TScriptInterface<ICommandSeeker> GetCommandSeeker();
	virtual TScriptInterface<ICommandSeeker> GetCommandSeeker_Implementation();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "System Context")
	FUserInfo GetUserInfo(const int InUserID);
	virtual FUserInfo GetUserInfo_Implementation(const int InUserID);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "System Context")
	void ShowWindowOnWorkspace(const UProgram* InProgram);
	virtual void ShowWindowOnWorkspace_Implementation(const UProgram* InProgram);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "System Context")
	EUserDomain GetUserDomain(int InUserID);
	virtual EUserDomain GetUserDomain_Implementation(int InUserID);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "System Context")
	FText GetUsername(int InUserID);
	virtual FText GetUsername_Implementation(int InUserID);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "System Context")
	FString GetUserHomeDirectory(int UserID);
	virtual FString GetUserHomeDirectory_Implementation(int UserID);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hacking")
	bool Authenticate(const FString& Username, const FString& Password, int& UserID);
	virtual bool Authenticate_Implementation(const FString& Username, const FString& Password, int& UserID);

	UFUNCTION(BlueprintImplementableEvent, Category = "Peacegate")
	bool GetSuitableProgramForFileExtension(const FString& InExtension, class UPeacegateProgramAsset*& OutProgram);
	virtual bool GetSuitableProgramForFileExtension_Implementation(const FString& InExtension, class UPeacegateProgramAsset*& OutProgram);

	/*** IFolderRepository ***/

	UFUNCTION(BlueprintImplementableEvent, Category = "Save System")
	void GetFolderTree(TArray<FFolder>& OutFolderTree);
	virtual void GetFolderTree_Implementation(TArray<FFolder>& OutFolderTree);

	UFUNCTION(BlueprintImplementableEvent, Category = "Save System")
	void PushFolderTree(const TArray<FFolder>& InFolderTree);
	virtual void PushFolderTree_Implementation(const TArray<FFolder>& InFolderTree);
};
