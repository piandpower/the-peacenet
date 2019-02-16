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
#include "UDatabaseParser.h"
#include "UPeacegateFileSystem.h"
#include "UDatabase.generated.h"

/**
 * Encapsulates a Peacenet database.
 */
UCLASS(BlueprintType)
class PROJECTOGLOWIA_API UDatabase : public UObject
{
	GENERATED_BODY()

public: // Properties.
	UPROPERTY()
	TArray<FDatabaseTable> Tables;

public: // Member functions.
	UFUNCTION(BlueprintCallable, Category = "Database")
	void AddTable(FString TableName);

	UFUNCTION(BlueprintCallable, Category = "Database")
	void AddColumnToTable(FString TableName, FString ColumnName);

	UFUNCTION(BlueprintCallable, Category = "Database")
		void AddRowToTableChecked(FString InTable, TMap<FString, FString> InRow);

	UFUNCTION(BlueprintCallable, Category = "Database", BlueprintPure)
	bool TableExists(FString InTable);

	UFUNCTION(BlueprintCallable, Category = "Database", BlueprintPure)
	bool ColumnExistsInTable(FString InTable, FString InColumn);

	UFUNCTION(BlueprintCallable, Category = "Database", BlueprintPure)
	TArray<FString> GetTables();

	UFUNCTION(BlueprintCallable, Category = "Database", BlueprintPure)
	int GetColumnCount(FString InTable);

	UFUNCTION(BlueprintCallable, Category = "Database", BlueprintPure)
	TArray<FString> GetColumns(FString InTable);

	UFUNCTION(BlueprintCallable, Category = "Database", BlueprintPure)
	TArray<FString> GetColumnValues(FString InTable, FString InColumn);

public: // Static functions.
	UFUNCTION(BlueprintCallable, Category = "Database")
	static bool ReadFromFile(UPeacegateFileSystem* InFilesystem, FString InPath, EFilesystemStatusCode& StatusCode, UDatabase*& OutDatabase);
};
