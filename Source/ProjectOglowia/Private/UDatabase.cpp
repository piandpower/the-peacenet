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

bool UDatabase::TableExists(FString InTable)
{
	return this->GetTables().Contains(InTable);
}

bool UDatabase::ColumnExistsInTable(FString InTable, FString InColumn)
{
	return TableExists(InTable) && GetColumns(InTable).Contains(InColumn);
}

void UDatabase::AddTable(FString TableName)
{
	check(!TableExists(TableName));

	FDatabaseTable NewTable;
	NewTable.Name = TableName;
	NewTable.Columns.Add("ID");

	this->Tables.Add(NewTable);
}

void UDatabase::AddColumnToTable(FString TableName, FString ColumnName)
{
	check(TableExists(TableName));

	if (!ColumnExistsInTable(TableName, ColumnName))
	{
		for (FDatabaseTable& Table : Tables)
		{
			if (Table.Name == TableName)
			{
				Table.Columns.Add(ColumnName);

				// Now we need to update all the existing rows.
				for (FDatabaseRow& Row : Table.Rows)
				{
					Row.Columns.Add(ColumnName, "");
				}
			}
		}
	}
}