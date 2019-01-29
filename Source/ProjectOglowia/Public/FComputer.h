// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "FFolder.h"
#include "Engine/Texture2D.h"
#include "EComputerType.h"
#include "FFirewallRule.h"
#include "FComputer.generated.h"

class UVulnerability;

/**
 * Represents a user's permission and authentication domain, dictating what the user can do and how they authenticate.
 */
UENUM(BlueprintType)
enum class EUserDomain : uint8
{
	Administrator,
	PowerUser,
	User,
	FileTransferUser,
	VaultUser,
	MailboxUser,
	DatabaseUser,
	Anonymous
};

USTRUCT(BlueprintType)
struct PROJECTOGLOWIA_API FServiceInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Hackable")
	FName ServiceName;

	UPROPERTY(BlueprintReadOnly, Category = "Hackable")
	int Port;
};

/**
 * Represents a Peacegate user.
 */
USTRUCT(BlueprintType)
struct PROJECTOGLOWIA_API FUser
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int ID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EUserDomain Domain;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Username;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Password;
};

/**
 * Defines how the game should handle this computer.
 */
UENUM(BlueprintType)
enum class EComputerOwnerType : uint8
{
	Player,
	NPC,
	Story
};

/**
 * Represents a computer within The Peacenet.
 */
USTRUCT(BlueprintType)
struct PROJECTOGLOWIA_API FComputer
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Computer")
	int ID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Computer")
	EComputerOwnerType OwnerType = EComputerOwnerType::NPC;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Computer")
	EComputerType ComputerType = EComputerType::Personal;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Computer")
	TArray<FFolder> Filesystem;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Computer")
	TArray<FUser> Users;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Computer")
	FName WallpaperID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Computer")
	TArray<FName> InstalledPrograms;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Computer")
	TArray<FName> InstalledCommands;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Hacking")
	TArray<FServiceInfo> ActiveServices;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Computer")
	UTexture2D* CurrentWallpaper;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Unlocks")
	TArray<FName> UnlockedWallpapers;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Firewall")
	TArray<FFirewallRule> FirewallRules;
};