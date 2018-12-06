// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "FComputer.h"
#include "UPeacegateFileSystem.h"
#include "FPeacenetIdentity.h"
#include "FNetMapScanEventArgs.h"
#include "USystemContext.generated.h"


class UDesktopWidget;
class APeacenetWorldStateActor;
class UPeacegateProgramAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FNetMapScanEvent, int, EntityID, FNetMapScanEventArgs, Args);

USTRUCT(BlueprintType)
struct PROJECTOGLOWIA_API FUserInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FString Username;

	UPROPERTY(BlueprintReadOnly)
	bool IsAdminUser = false;
};

/**
 * Represents the state and allows access/modification of an NPC or player computer in Peacenet.
 */
UCLASS(BlueprintType)
class PROJECTOGLOWIA_API USystemContext : public UObject
{
	GENERATED_BODY()

private:
	FString CurrentHostname;

public:
	UPROPERTY()
	FNetMapScanEvent NetMapScan;

	UFUNCTION()
	void BroadcastNetMapEvent(int InEntityID, FNetMapScanEventArgs EventArgs);

	UFUNCTION()
	void ExecuteCommand(FString InCommand);

	UPROPERTY()
	APeacenetWorldStateActor * Peacenet;
	
	UPROPERTY(BlueprintReadOnly)
	FComputer Computer;
	
	UPROPERTY(BlueprintReadOnly)
	FPeacenetIdentity Character;

	UPROPERTY(BlueprintReadOnly)
	UDesktopWidget* Desktop;

protected:
	UFUNCTION()
		void HandleFileSystemEvent(EFilesystemEventType InType, FString InPath);

public:
	UFUNCTION()
	void UpdateSystemFiles();

	UFUNCTION(BlueprintCallable, Category = "System Context")
	FString GetHostname();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "System Context")
	TArray<UPeacegateProgramAsset*> GetInstalledPrograms();

	UFUNCTION(BlueprintCallable, Category = "System Context")
	bool OpenProgram(FName InExecutableName);

	UFUNCTION(BlueprintCallable, Category = "System Context")
		UPeacegateFileSystem* GetFilesystem(const int UserID);

	UFUNCTION()
		bool TryGetTerminalCommand(FName CommandName, UTerminalCommand*& OutCommand, FString& InternalUsage, FString& FriendlyUsage);

	UPROPERTY()
	TMap<int, UPeacegateFileSystem*> RegisteredFilesystems;

	UFUNCTION(BlueprintCallable, Category = "System Context")
		FUserInfo GetUserInfo(const int InUserID);

	UFUNCTION()
	void LogEvent(int UserID, FString Message);

	UFUNCTION(BlueprintCallable, Category = "System Context")
		void ShowWindowOnWorkspace(UProgram* InProgram);

	UFUNCTION(BlueprintCallable, Category = "System Context")
		EUserDomain GetUserDomain(int InUserID);

	UFUNCTION(BlueprintCallable, Category = "System Context")
		FText GetUsername(int InUserID);

	UFUNCTION(BlueprintCallable, Category = "System Context")
		FString GetUserHomeDirectory(int UserID);

	UFUNCTION(BlueprintCallable, Category = "Hacking")
		bool Authenticate(const FString& Username, const FString& Password, int& UserID);

	UFUNCTION(Category = "Peacegate")
		bool GetSuitableProgramForFileExtension(const FString& InExtension, class UPeacegateProgramAsset*& OutProgram);

	UFUNCTION()
		void GetFolderTree(TArray<FFolder>& OutFolderTree);

	UFUNCTION()
		void PushFolderTree(const TArray<FFolder>& InFolderTree);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Peacegate")
	FText GetTimeOfDay();

	UFUNCTION(BlueprintCallable, Category = "Peacegate|Setup")
	static void ParseCharacterName(const FString InCharacterName, FString& OutUsername, FString& OutHostname);
};
