// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UDatabase.h"

bool UDatabase::ReadFromFile(UPeacegateFileSystem * InFilesystem, FString InPath, EFilesystemStatusCode & StatusCode, UDatabase *& OutDatabase)
{
	if (!InFilesystem->FileExists(InPath))
	{
		StatusCode = EFilesystemStatusCode::FileOrDirectoryNotFound;
		return false;
	}

	FString DbText;

	InFilesystem->ReadText(InPath, DbText, StatusCode);

	if (StatusCode != EFilesystemStatusCode::OK)
	{
		return false;
	}

	OutDatabase = NewObject<UDatabase>();

	OutDatabase->Tables = UDatabaseParser::ParseTables(DbText);

	return true;
}

TArray<FString> UDatabase::GetTables()
{
	TArray<FString> TableNames;
	for (auto& Table : this->Tables)
	{
		TableNames.Add(Table.Name);
	}
	return TableNames;
}

int UDatabase::GetColumnCount(FString InTable)
{
	for (auto& Table : this->Tables)
	{
		if (Table.Name == InTable)
		{
			return Table.Columns.Num();
		}
	}
	return 0;
}

TArray<FString> UDatabase::GetColumns(FString InTable)
{
	for (auto& Table : this->Tables)
	{
		if (Table.Name == InTable)
		{
			return Table.Columns;
		}
	}
	return TArray<FString>();
}

TArray<FString> UDatabase::GetColumnValues(FString InTable, FString InColumn)
{
	TArray<FString> Values;

	for (auto& Table : this->Tables)
	{
		if (Table.Name == InTable)
		{
			for (auto& Row : Table.Rows)
			{
				if (Row.Columns.Contains(InColumn))
				{
					Values.Add(Row.Columns[InColumn]);
				}
			}
		}
	}

	return Values;
}
