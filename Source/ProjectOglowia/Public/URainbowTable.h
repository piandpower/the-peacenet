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
