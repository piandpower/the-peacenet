/********************************************************************************
 * The Peacenet - bit::phoenix("software");
 * 
 * MIT License
 *
 * Copyright (c) 2018-2019 Michael VanOverbeek, Declan Hoare, Ian Clary, 
 * Trey Smith, Richard Moch, Victor Tran and Warren Harris
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * Contributors:
 *  - Michael VanOverbeek <alkaline@bitphoenixsoftware.com>
 *
 ********************************************************************************/


#pragma once

#include "CoreMinimal.h"
#include "FComputer.h"
#include "UPeacegateFileSystem.h"
#include "FPeacenetIdentity.h"
#include "FAdjacentNodeInfo.h"
#include "USystemContext.generated.h"

class UHackable;
class UDesktopWidget;
class UProgram;
class URainbowTable;
class APeacenetWorldStateActor;
class UPeacegateProgramAsset;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSystemConnectedEvent, UHackable*, InConnection, bool, IsInbound);

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

protected:
	UPROPERTY()
	FString CurrentHostname;

	UPROPERTY()
	TMap<int, UPeacegateFileSystem*> RegisteredFilesystems;

	UPROPERTY()
	URainbowTable* RainbowTable;

	UPROPERTY()
	APeacenetWorldStateActor * Peacenet;

	UPROPERTY()
	int ComputerID = 0;
	
	UPROPERTY()
	int CharacterID = 0;

	UPROPERTY()
	UDesktopWidget* Desktop;

	UPROPERTY()
	TMap<int, UUserContext*> Users;

	UPROPERTY()
	TArray<UHackable*> OutboundConnections;

	UPROPERTY()
	TArray<UHackable*> InboundConnections;

public:
	UPROPERTY(BlueprintAssignable, Category = "Networking")
	FSystemConnectedEvent SystemConnected;

protected:
	UFUNCTION()
	void HandleFileSystemEvent(EFilesystemEventType InType, FString InPath);

public: // Property getters
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "System Context")
	APeacenetWorldStateActor* GetPeacenet();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "System Context")
	TArray<UWallpaperAsset*> GetAvailableWallpapers();

	UFUNCTION(BlueprintCallable, Category = "System Context")
	void SetCurrentWallpaper(UWallpaperAsset* InWallpaperAsset);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "System Context")
	UDesktopWidget* GetDesktop();

	UFUNCTION()
	FPeacenetIdentity& GetCharacter();

	UFUNCTION()
	FComputer& GetComputer();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "System Context")
	URainbowTable* GetRainbowTable();

	UFUNCTION(BlueprintCallable, Category = "System Context")
	FString GetHostname();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "System Context")
	TArray<UPeacegateProgramAsset*> GetInstalledPrograms();

public:
	UFUNCTION()
	int GetOpenConnectionCount();

	UFUNCTION()
	void AddConnection(UHackable* InConnection, bool IsInbound);

	UFUNCTION()
	bool UsernameExists(FString InUsername);

	UFUNCTION()
	void Disconnect(UHackable* InConnection);

	UFUNCTION()
	bool IsIPAddress(FString InIPAddress);

	UFUNCTION()
	void SetupDesktop(int InUserID);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Adjacent nodes")
	TArray<FAdjacentNodeInfo> ScanForAdjacentNodes();

	UFUNCTION()
	TArray<UCommandInfo*> GetInstalledCommands();

	UFUNCTION()
	void Setup(int InComputerID, int InCharacterID, APeacenetWorldStateActor* InPeacenet);

	UFUNCTION()
	void ExecuteCommand(FString InCommand);

	UFUNCTION()
	void UpdateSystemFiles();

	UFUNCTION()
	bool OpenProgram(FName InExecutableName, UProgram*& OutProgram, bool InCheckForExistingWindow = true);

	UFUNCTION()
	UUserContext* GetUserContext(int InUserID);

	UFUNCTION()
	UPeacegateFileSystem* GetFilesystem(const int UserID);

	UFUNCTION()
	bool TryGetTerminalCommand(FName CommandName, ATerminalCommand*& OutCommand, FString& InternalUsage, FString& FriendlyUsage);

	UFUNCTION()
	FString GetIPAddress();

	UFUNCTION()
	FUserInfo GetUserInfo(const int InUserID);

	UFUNCTION()
	void ShowWindowOnWorkspace(UProgram* InProgram);

	UFUNCTION()
	EUserDomain GetUserDomain(int InUserID);

	UFUNCTION()
	FString GetUsername(int InUserID);

	UFUNCTION()
	FString GetUserHomeDirectory(int UserID);

	UFUNCTION()
	bool Authenticate(const FString& Username, const FString& Password, int& UserID);

	UFUNCTION()
	bool GetSuitableProgramForFileExtension(const FString& InExtension, class UPeacegateProgramAsset*& OutProgram);

	UFUNCTION()
	void GetFolderTree(TArray<FFolder>& OutFolderTree);

	UFUNCTION()
	void PushFolderTree(const TArray<FFolder>& InFolderTree);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Peacegate")
	FText GetTimeOfDay();
};
