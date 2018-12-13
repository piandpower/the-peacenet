// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

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
