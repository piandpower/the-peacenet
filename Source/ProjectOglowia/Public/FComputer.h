// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "FFolder.h"
#include "FComputer.generated.h"

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
	DatabaseUser
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
	FText Username;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText Password;
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
	FText Hostname;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Computer")
	FName ComputerType = NAME_None;

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
};