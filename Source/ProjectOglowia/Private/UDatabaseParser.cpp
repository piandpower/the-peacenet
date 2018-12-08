// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UDatabaseParser.h"

TArray<FDatabaseTable> UDatabaseParser::ParseTables(FString InTableString)
{
	FString CurrentTableName;
	TArray<FString> ColumnNames;
	TArray<FDatabaseRow> Rows;

	check(!InTableString.IsEmpty());

	TArray<FDatabaseTable> Tables;

	TArray<FString> Lines;
	InTableString.ParseIntoArray(Lines, TEXT("\n"), true);

	for (auto Line : Lines)
	{
		if (Line.TrimStartAndEnd().IsEmpty())
			continue;

		if (Line.StartsWith("\t"))
		{
			check(!CurrentTableName.IsEmpty());
			
			if (!ColumnNames.Num())
			{
				Line.ParseIntoArray(ColumnNames, TEXT("\t"), true);
				check(ColumnNames.Num());

				TArray<FString> ColumnCheck;
				for (auto Column : ColumnNames)
				{
					check(!ColumnCheck.Contains(Column));
					ColumnCheck.Add(Column);
				}
			}
			else
			{
				TArray<FString> Row;
				Line.ParseIntoArray(Row, TEXT("\t"), true);
				check(Row.Num() == ColumnNames.Num());
				
				TMap<FString, FString> ColumnMap;
				
				for (int i = 0; i < Row.Num(); i++)
				{
					ColumnMap.Add(ColumnNames[i], Row[i]);
				}

				FDatabaseRow NewRow;
				NewRow.Columns = ColumnMap;
				Rows.Add(NewRow);

			}
		}
		else
		{
			if (!CurrentTableName.IsEmpty())
			{
				check(ColumnNames.Num());

				FDatabaseTable NewTable;
				NewTable.Name = CurrentTableName;
				NewTable.Rows = Rows;

				Tables.Add(NewTable);
			}

			CurrentTableName = Line.TrimStartAndEnd();
			ColumnNames.Empty();
			Rows.Empty();
		}
	}

	return Tables;
}

FString UDatabaseParser::SerializeDatabase(TArray<FDatabaseTable> InDatabase)
{
	FString Serialized;

	for (auto& Table : InDatabase)
	{
		Serialized.Append(Table.Name + "\n");
		for (auto Column : Table.Columns)
		{
			Serialized.Append("\t" + Column);
		}
		Serialized.Append("\n");
		for (auto& Row : Table.Rows)
		{
			for (auto Column : Table.Columns)
			{
				Serialized.Append("\t" + Row.Columns[Column]);
			}
			Serialized.Append("\n");
		}
		Serialized.Append("\n");
	}

	return Serialized;
}
