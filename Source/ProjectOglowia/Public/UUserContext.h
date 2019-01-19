// Copyright (c) 2018 Alkaline Thunder & The Peacenet

#pragma once

#include "CoreMinimal.h"
#include "USystemContext.h"
#include "EFileOpenResult.h"
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
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "User Context")
    TArray<UWallpaperAsset*> GetAvailableWallpapers();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "User Context")
    UTexture2D* GetCurrentWallpaper();

    UFUNCTION(BlueprintCallable, Category = "User Context")
    void SetCurrentWallpaper(UWallpaperAsset* InWallpaperAsset);

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
    void ShowProgramOnWorkspace(UProgram* InProgram);

    UFUNCTION(BlueprintCallable, Category = "User Context")
    UConsoleContext* CreateConsole(UPTerminalWidget* InTerminalWidget);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "User Context")
    FString GetUserTypeDisplay();

	UFUNCTION(BlueprintCallable, Category = "Program")
	bool OpenFile(const FString& InPath, EFileOpenResult& OutResult);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "User Context")
    bool IsAdministrator();
};