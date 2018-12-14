// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "UDatabase.h"
#include "UDatabaseParser.h"
#include "URainbowTable.generated.h"

class USystemContext;
class UPeacegateFileSystem;

/**
 * Contains functionality for reading from/writing to a Peacenet Rainbow Table file.
 */
UCLASS(BlueprintType)
class PROJECTOGLOWIA_API URainbowTable : public UObject
{
	GENERATED_BODY()

	// If there's anything I've learned from GAME102 when it comes to OOP, it's that:
	// 1. It really stands for "Overly Obfuscated Projects."
	// 2. Encapsulation.
	//
	// - Alkaline Thunder

protected:
	// The system that owns this rainbow table manager.
	UPROPERTY()
	USystemContext* SystemContext;

	// The filesystem, preferrably with root permissions, that we'll use to read the rainbow table.
	UPROPERTY()
	UPeacegateFileSystem* Filesystem;

	// The path to the rainbow table.
	UPROPERTY()
	FString RainbowTablePath;

	// This is the underlying database object that holds the actual rainbow table data.
	UPROPERTY()
	UDatabase* RainbowTable;

	// Should the game auto-flush this RainboW table?
	UPROPERTY()
	bool ShouldAutoFlush = true;

protected:
	UFUNCTION()
	void UpdateTableFormat();

	UFUNCTION()
	void ReloadTable();

public:
	// Flushes the contents of the Rainbow Table to the filesystem.
	UFUNCTION()
	void Flush();

	// Set up this rainbow table manager with the given system context and file path.
	UFUNCTION()
	void Setup(USystemContext* InSystem, FString InPath, bool InShouldAutoFlush);

	// Adds a password to the rainbow table if it does not exist.
	UFUNCTION()
	void AddPassword(FString InPassword);
};
