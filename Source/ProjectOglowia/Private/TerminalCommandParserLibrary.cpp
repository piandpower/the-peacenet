// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "TerminalCommandParserLibrary.h"

FPeacegateCommandInstruction UTerminalCommandParserLibrary::GetCommandList(const FString& InCommand, FString& OutputError)
{
	//This is the list of commands to run in series
	TArray<FString> commands;

	//Parser state
	bool escaping = false;
	bool inQuote = false;
	FString current = TEXT("");
	bool isFileName = false;

	//output file name
	FString fileName = TEXT("");

	//Will the game overwrite the specified file with output?
	bool shouldOverwriteOnFileRedirect = false;

	//Length of the input command.
	int cmdLength = InCommand.Len();

	//Iterate through each character in the command.
	for (int i = 0; i < cmdLength; i++)
	{
		//Get the character at the current index.
		TCHAR c = InCommand[i];

		//If we're a backslash, parse an escape sequence.
		if (c == TEXT('\\'))
		{
			//Ignore escape if we're not in a quote or file.
			if (!(inQuote || isFileName))
			{
				current.AppendChar(c);
				continue;
			}
			//If we're not currently escaping...
			if (!escaping)
			{
				escaping = true;
				//If we're a filename, append to the filename string.
				if (isFileName)
				{
					fileName.AppendChar(c);
				}
				else
				{
					current.AppendChar(c);
				}
				continue;
			}
			else
			{
				escaping = false;
			}
		}
		else if (c == TEXT('"'))
		{
			if (!isFileName)
			{
				if (!escaping)
				{
					inQuote = !inQuote;
				}
			}
		}
		if (c == TEXT('|'))
		{
			if (!isFileName)
			{
				if (!inQuote)
				{
					if (current.TrimStartAndEnd().IsEmpty())
					{
						OutputError = TEXT("unexpected token '|' (pipe)");
						return FPeacegateCommandInstruction::Empty();
					}
					commands.Add(current.TrimStartAndEnd());
					current = TEXT("");
					continue;
				}
			}
		}
		else if (FChar::IsWhitespace(c))
		{
			if (isFileName)
			{
				if (!escaping)
				{
					if (fileName.IsEmpty())
					{
						continue;
					}
					else
					{
						OutputError = TEXT("unexpected whitespace in filename.");
						return FPeacegateCommandInstruction::Empty();
					}
				}
			}
		}
		else if (c == TEXT('>'))
		{
			if (!isFileName)
			{
				isFileName = true;
				shouldOverwriteOnFileRedirect = false;
				continue;
			}
			else
			{
				if (InCommand[i - 1] == TEXT('>'))
				{
					if (!shouldOverwriteOnFileRedirect)
					{
						shouldOverwriteOnFileRedirect = true;
					}
					else {
						OutputError = TEXT("unexpected token '>' (redirect) in filename");
						return FPeacegateCommandInstruction::Empty();
					}
					continue;
				}
			}
		}
		if (isFileName)
			fileName.AppendChar(c);
		else
			current.AppendChar(c);
		if (escaping)
			escaping = false;

	}
	if (inQuote)
	{
		OutputError = TEXT("expected closing quotation mark, got end of command.");
		return FPeacegateCommandInstruction::Empty();
	}
	if (escaping)
	{
		OutputError = TEXT("expected escape sequence, got end of command.");
		return FPeacegateCommandInstruction::Empty();
	}
	if (!current.IsEmpty())
	{
		commands.Add(current.TrimStartAndEnd());
		current = TEXT("");
	}
	return FPeacegateCommandInstruction(commands, fileName, shouldOverwriteOnFileRedirect);
}

TArray<FString> UTerminalCommandParserLibrary::Tokenize(const FString& InCommand, const FString& Home, FString& OutputError) 
{
	TArray<FString> tokens;
	FString current = TEXT("");
	bool escaping = false;
	bool inQuote = false;

	int cmdLength = InCommand.Len();

	TArray<TCHAR> cmd = InCommand.GetCharArray();

	for (int i = 0; i < cmdLength; i++)
	{
		TCHAR c = cmd[i];
		if (c == TEXT('\\'))
		{
			if (escaping == false)
				escaping = true;
			else
			{
				escaping = false;
				current.AppendChar(c);
			}
			continue;
		}
		if (escaping == true)
		{
			switch (c)
			{
			case TEXT(' '):
				current.AppendChar(TEXT(' '));
				break;
			case TEXT('~'):
				current.AppendChar(TEXT('~'));
				break;
			case TEXT('n'):
				current.AppendChar(TEXT('\n'));
				break;
			case TEXT('r'):
				current.AppendChar(TEXT('\r'));
				break;
			case TEXT('t'):
				current.AppendChar(TEXT('\t'));
				break;
			case TEXT('"'):
				current.AppendChar(TEXT('"'));
				break;
			default:
				OutputError = TEXT("unrecognized escape sequence.");
				return TArray<FString>();
			}
			escaping = false;
			continue;
		}
		if (c == TEXT('~'))
		{
			if (inQuote == false && current.IsEmpty())
			{
				current = current.Append(Home);
				continue;
			}
		}
		if (c == TEXT(' '))
		{
			if (inQuote)
			{
				current.AppendChar(c);
			}
			else
			{
				if (!current.IsEmpty())
				{
					tokens.Add(current);
					current = TEXT("");
				}
			}
			continue;
		}
		if (c == TEXT('"'))
		{
			inQuote = !inQuote;
			if (!inQuote)
			{
				if (i + 1 < cmdLength)
				{
					if (cmd[i + 1] == TEXT('"'))
					{
						OutputError = TEXT("String splice detected. Did you mean to use a literal double-quote (\\\")?");
						return TArray<FString>();
					}
				}
			}
			continue;
		}
		current.AppendChar(c);
	}
	if (inQuote)
	{
		OutputError = TEXT("expected ending double-quote, got end of command instead.");
		return TArray<FString>();
	}
	if (escaping)
	{
		OutputError = TEXT("expected escape sequence, got end of command instead.");
		return TArray<FString>();
	}
	if (!current.IsEmpty())
	{
		tokens.Add(current);
		current = TEXT("");
	}
	return tokens;
}
