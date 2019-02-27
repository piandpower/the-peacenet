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
#include "USystemContext.h"
#include "EFileOpenResult.h"
#include "FPeacegateProcess.h"
#include "SecureShell.h"
#include "FAdjacentNodeInfo.h"
#include "UUserContext.generated.h"

class UPeacegateFileSystem;
class APeacenetWorldStateActor;
class UAddressBookContext;
class URainbowTable;
class UPTerminalWidget;
class UComputerService;
class UDesktopWidget;
class UVulnerability;
class UProgram;
class UConsoleContext;

/**
 * A System Context that acts as a specific user.
 */
UCLASS(BlueprintType)
class PROJECTOGLOWIA_API UUserContext : public UObject
{
    GENERATED_BODY()

private:
    // The owning system context.
    UPROPERTY()
    USystemContext* OwningSystem = nullptr;

    // The user ID of the user who owns this system context.
    UPROPERTY()
    int UserID = 0;

public:
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "URL parsing")
    void ParseURL(FString InURL, int InDefaultPort, FString& OutUsername, FString& OutHost, int& OutPort, FString& OutPath);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Adjacent nodes")
    TArray<FAdjacentNodeInfo> ScanForAdjacentNodes();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "User Context")
    TArray<UWallpaperAsset*> GetAvailableWallpapers();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "User Context")
    UTexture2D* GetCurrentWallpaper();

    UFUNCTION(BlueprintCallable, Category = "User Context")
    void SetCurrentWallpaper(UWallpaperAsset* InWallpaperAsset);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Process List")
    TArray<FPeacegateProcess> GetRunningProcesses();

    UFUNCTION()
    FUserInfo GetUserInfo();

    UFUNCTION()
    void Setup(USystemContext* InOwningSystem, int InUserID);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "User Context")
    FString GetUsername();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "User Context")
    FString GetHostname();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "User Context")
    FString GetHomeDirectory();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "User Context")
    FString GetCharacterName();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "User Context")
    UPeacegateFileSystem* GetFilesystem();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "User Context")
    URainbowTable* GetRainbowTable();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "User Context")
    APeacenetWorldStateActor* GetPeacenet();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "User Context")
    UDesktopWidget* GetDesktop();

    UFUNCTION()
    USystemContext* GetOwningSystem();

    UFUNCTION(BlueprintCallable, Category = "User Context")
    bool OpenProgram(FName InExecutableName, UProgram*& OutProgram, bool InCheckForExistingWindow = true);

    UFUNCTION()
    int StartProcess(FString Name, FString FilePath);

    UFUNCTION()
    void ShowProgramOnWorkspace(UProgram* InProgram);

    UFUNCTION(BlueprintCallable, Category = "User Context")
    UConsoleContext* CreateConsole(UPTerminalWidget* InTerminalWidget);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "User Context")
    FString GetUserTypeDisplay();

	UFUNCTION(BlueprintCallable, Category = "Program")
	bool OpenFile(const FString& InPath, EFileOpenResult& OutResult);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "User Context")
    bool IsAdministrator();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Process List")
    FString GetProcessUsername(FPeacegateProcess InProcess);
};