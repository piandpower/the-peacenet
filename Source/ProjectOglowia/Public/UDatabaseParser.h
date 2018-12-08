// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "UDatabaseParser.generated.h"

USTRUCT()
struct PROJECTOGLOWIA_API FDatabaseRow
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TMap<FString, FString> Columns;
};

USTRUCT()
struct PROJECTOGLOWIA_API FDatabaseTable
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString Name;

	UPROPERTY()
	TArray<FString> Columns;

	UPROPERTY()
	TArray<FDatabaseRow> Rows;
};



/**
 * A class for parsing a Peacenet .db file.
 */
UCLASS(BlueprintType)
class PROJECTOGLOWIA_API UDatabaseParser : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
	static TArray<FDatabaseTable> ParseTables(FString InTableString);

	UFUNCTION()
	static FString SerializeDatabase(TArray<FDatabaseTable> InDatabase);
};
