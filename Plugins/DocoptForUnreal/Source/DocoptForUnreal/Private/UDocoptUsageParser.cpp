// Copyright (c) 2018 The Peacenet & Alkaline Thunder.


#include "UDocoptUsageParser.h"
#include <regex>
#include "UDocoptUtils.h"

UTokens* UTokens::FromPattern(FString InSource)
{
    // TODO: Actually do stuff.
    UTokens* NewTokens = NewObject<UTokens>();

	static const std::regex re_separators{
			"(?:\\s*)" // any spaces (non-matching subgroup)
			"("
			"[\\[\\]\\(\\)\\|]" // one character of brackets or parens or pipe character
			"|"
			"\\.\\.\\."  // elipsis
			")" };

	static const std::regex re_strings{
		"(?:\\s*)" // any spaces (non-matching subgroup)
		"("
		"\\S*<.*?>"  // strings, but make sure to keep "< >" strings together
		"|"
		"[^<>\\s]+"     // string without <>
		")" };

	// We do two stages of regex matching. The '[]()' and '...' are strong delimeters
	// and need to be split out anywhere they occur (even at the end of a token). We
	// first split on those, and then parse the stuff between them to find the string
	// tokens. This is a little harder than the python version, since they have regex.split
	// and we dont have anything like that.

	std::string source = TCHAR_TO_UTF8(*InSource);

	TArray<FString> tokens;
	std::for_each(std::sregex_iterator{ source.begin(), source.end(), re_separators },
		std::sregex_iterator{},
		[&](std::smatch const& match)
	{
		// handle anything before the separator (this is the "stuff" between the delimeters)
		if (match.prefix().matched) {
			std::for_each(std::sregex_iterator{ match.prefix().first, match.prefix().second, re_strings },
				std::sregex_iterator{},
				[&](std::smatch const& m)
			{
				tokens.Add(FString(m[1].str().c_str()));
			});
		}

		// handle the delimter token itself
		if (match[1].matched) {
			tokens.Add(FString(match[1].str().c_str()));
		}
	});

	NewTokens->TokensArray = tokens;
	NewTokens->IsParsingArgv = false;

	return NewTokens;
}

FString UTokens::Pop()
{
    FString At = this->TokensArray[Index];
    Index++;
    return At;
}

FString UTokens::Current()
{
    if(*this)
    {
        return TokensArray[Index];
    }
    return FString();
}

FString UTokens::TheRest()
{
    if(!*this)
    {
        return FString();
    }
    return UDocoptUtils::Join(UDocoptUtils::SubArray(TokensArray, Index, TokensArray.Num()), TEXT(" "));
}

bool UDocoptUsageParser::CreatePatternTree(FString InDoc, FSyntaxTree& OutSyntaxTree, FString& OutError)
{
	auto usageSections = ParseSection(TEXT("usage:"), InDoc);

	if(usageSections.Num() < 1)
	{
		OutError = TEXT("docopt language error: undefined documentation section 'usage'.");
		return false;
	}
	else if(usageSections.Num() > 1)
	{
		OutError = TEXT("docopt language error: multiple definitions of documentation section 'usage'.");
		return false;
	}

	FString err;

	TArray<UOption*> options = ParseDefaults(InDoc);
	URequired* pattern = ParsePattern(FormatUsage(usageSections[0]), options, err);
	
	if(!pattern)
	{
		OutError = err;
		return false;
	}

	TArray<UOption*> patternOptions = FlatFilter<UOption>(pattern);

	TArray<int> usedHashes;
	TArray<UOption*> uniqueOptions;

	for(auto& opt : patternOptions)
	{
		int hash = opt->HashPattern();
		if(!usedHashes.Contains(hash))
		{
			usedHashes.Add(hash);
			uniqueOptions.Add(opt);
		}
	}

	for(auto optionsShortcut : FlatFilter<UOptionsShortcut>(pattern))
	{
		TArray<UOption*> docOptions = ParseDefaults(InDoc);

		PatternList children;
		for(auto& opt : docOptions)
		{
			if(!usedHashes.Contains(opt->HashPattern()))
			{
				children.Add(opt);
			}
		}

		optionsShortcut->Children = children;
	}

	OutSyntaxTree = FSyntaxTree(pattern, options);
	return true;
}

bool UDocoptUsageParser::Extras(bool InHelp, bool InVersion, PatternList& InOptions, FString& OutMessage)
{
	if(InHelp && IsOptionSet(InOptions, TEXT("-h"), TEXT("--help")))
	{
		OutMessage = TEXT("help");
		return true;
	}

	if(InHelp && IsOptionSet(InOptions, TEXT("-v"), TEXT("--version")))
	{
		OutMessage = TEXT("version");
		return true;
	}

	return false;
}

bool UDocoptUsageParser::IsOptionSet(PatternList& InOptions, FString InOpt1, FString InOpt2)
{
	for(auto opt : InOptions)
	{
		FString optName = opt->Name();
		if(optName == InOpt1 || (!InOpt2.IsEmpty() && optName==InOpt2))
		{
			return opt->HasValue();
		}
	}
	return false;
}

TArray<UOption*> UDocoptUsageParser::ParseDefaults(FString InDoc)
{
	static std::regex const re_delimiter{
		"(?:^|\\n)[ \\t]*"  // a new line with leading whitespace
		"(?=-{1,2})"        // [split happens here] (positive lookahead) ... and followed by one or two hyphes
	};

	TArray<UOption*> ret;

	for (auto s : ParseSection(TEXT("options:"), InDoc)) 
	{
		s.RemoveAt(0, s.Find(":"));

		for (const auto& opt : UDocoptUtils::RegexSplit(s, re_delimiter)) 
		{
			if (UDocoptUtils::StartsWith(opt, TEXT("-")))
			{
				ret.Add(UOption::Parse(opt));
			}
		}
	}

	return ret;
}

PatternList UDocoptUsageParser::ParseArgv(UTokens* InTokens, TArray<UOption*>& OutOptions, bool InOptionsFirst, FString& OutError)
{
	PatternList ret;
	while(*InTokens)
	{
		FString token = InTokens->Current();
		if(token == TEXT("--"))
		{
			while(*InTokens)
			{
				UArgument* arg = NewObject<UArgument>();
				arg->LeafName = TEXT("");
				arg->SetValue(InTokens->Pop());
			}
		}
		else if(UDocoptUtils::StartsWith(token, TEXT("--")))
		{
			auto parsed = ParseLong(InTokens, OutOptions, OutError);
			if(parsed.Num() == 0)
			{
				return PatternList();
			}
			ret.Append(parsed);
		}
		else if(InOptionsFirst)
		{
			while(*InTokens)
			{
				UArgument* arg = NewObject<UArgument>();
				arg->LeafName = TEXT("");
				arg->SetValue(InTokens->Pop());
			}
		}
		else
		{
			UArgument* arg = NewObject<UArgument>();
			arg->LeafName = TEXT("");
			arg->SetValue(InTokens->Pop());
		}
	}
	return ret;
}

FString UDocoptUsageParser::FormatUsage(FString InSection)
{
	FString ret = TEXT("(");
	
	auto i = InSection.Find(":");
	auto parts = UDocoptUtils::Split(InSection, i);
	for(int j = 1; j < parts.Num(); j++)
	{
		if(parts[j] == parts[0])
		{
			ret.Append(TEXT(" ) | ("));
		}
		else
		{
			ret.Append(TEXT(" "));
			ret.Append(parts[j]);
		}
	}

	ret.Append(TEXT(" )"));
	return ret;
}

URequired* UDocoptUsageParser::ParsePattern(FString InSource, TArray<UOption*>& OutOptions, FString& OutError)
{
	auto tokens = UTokens::FromPattern(InSource);
	auto result = ParseExpression(tokens, OutOptions, OutError);

	if(result.Num()==0)
	{
		return nullptr;
	}

	if(*tokens)
	{
		OutError = TEXT("unexpected ending: '") + tokens->TheRest() + TEXT("'");
		return nullptr;
	}

	check(result.Num() == 1);

	URequired* ret = NewObject<URequired>();
	ret->Children = result;
	return ret;
}

PatternList UDocoptUsageParser::ParseExpression(UTokens* InTokens, TArray<UOption*>& OutOptions, FString& OutError)
{
	auto seq = ParseSequence(InTokens, OutOptions, OutError);
	if(seq.Num() == 0)
	{
		return PatternList();
	}

	if(InTokens->Current() != TEXT("|"))
	{
		return seq;
	}

	PatternList ret;
	ret.Add(TryCollapseToRequired(seq));

	while(InTokens->Current() == TEXT("|"))
	{
		InTokens->Pop();
		seq = ParseSequence(InTokens, OutOptions, OutError);
		if(seq.Num() == 0)
		{
			return PatternList();
		}
		ret.Add(TryCollapseToRequired(seq));
	}

	PatternList realRet;
	realRet.Add(TryCollapseToEither(ret));
	return realRet;
}

UPattern* UDocoptUsageParser::TryCollapseToEither(PatternList& InPatterns)
{
	if(InPatterns.Num()==0)
	{
		return InPatterns[0];
	}

	UEither* NewEither = NewObject<UEither>();
	NewEither->Children = InPatterns;
	return NewEither;
}

UPattern* UDocoptUsageParser::TryCollapseToRequired(PatternList& InPatterns)
{
	if(InPatterns.Num()==0)
	{
		return InPatterns[0];
	}

	URequired* NewReq = NewObject<URequired>();
	NewReq->Children = InPatterns;
	return NewReq;
}

PatternList UDocoptUsageParser::ParseSequence(UTokens* InTokens, TArray<UOption*>& OutOptions, FString& OutError)
{
	PatternList ret;

	while(*InTokens)
	{
		auto token = InTokens->Current();

		if(token == TEXT("]") || token == TEXT(")") || token == TEXT("|"))
		{
			break;
		}  

		auto atom = ParseAtom(InTokens, OutOptions, OutError);
		if(atom.Num() == 0)
		{
			// JUST STOP
			return atom;
		}

		if(InTokens->Current() == TEXT("..."))
		{
			UOneOrMore* oneOrMore = NewObject<UOneOrMore>();
			oneOrMore->Children = atom;
			ret.Add(oneOrMore);
			InTokens->Pop();
		}
		else
		{
			ret.Append(atom);
		}
	}

	return ret;
}

PatternList UDocoptUsageParser::ParseAtom(UTokens* InTokens, TArray<UOption*>& OutOptions, FString& OutError)
{
	PatternList ret;

	FString token = InTokens->Current();

	if(token == TEXT("["))
	{
		InTokens->Pop();

		auto exp = ParseExpression(InTokens, OutOptions, OutError);
		if(exp.Num()==0)
		{
			return exp;
		}

		auto trailing = InTokens->Pop();
		if(trailing != TEXT("]"))
		{
			OutError = TEXT("Mismatched '['.");
			return PatternList();
		}

		URequired* req = NewObject<URequired>();
		req->Children = exp;
		ret.Add(req);
	}
	else if(token == TEXT("options"))
	{
		InTokens->Pop();
		ret.Add(NewObject<UOptionsShortcut>());
	}
	else if(UDocoptUtils::StartsWith(token, TEXT("--")) && token != TEXT("--"))
	{
		ret = ParseLong(InTokens, OutOptions, OutError);
	}
	else if(UDocoptUtils::StartsWith(token, TEXT("-")) && token != TEXT("-") && token != TEXT("--"))
	{
		ret = ParseShort(InTokens, OutOptions, OutError);
	}
	else if(IsArgumentSpec(token))
	{
		UArgument* arg = NewObject<UArgument>();
		arg->LeafName = InTokens->Pop();
		ret.Add(arg);
	}
	else
	{
		UCommand* cmd = NewObject<UCommand>();
		cmd->LeafName = InTokens->Pop();
		ret.Add(cmd);
	}

	return ret;
}

PatternList UDocoptUsageParser::ParseShort(UTokens* InTokens, TArray<UOption*>& OutOptions, FString& OutError)
{
	auto token = InTokens->Pop();

	assert(UDocoptUtils::StartsWith(token, TEXT("-")));
	assert(!UDocoptUtils::StartsWith(token, TEXT("--")));

	int i = 1;

	PatternList ret;
	while (i != token.GetCharArray().Num())
	{
		FString shortOpt = TEXT("-") + FString::Chr(token[i]);
		i++;

		TArray<UOption*> similar;
		for(auto option : OutOptions) 
		{
			if (option->ShortOption == shortOpt)
			{
				similar.Add(option);
			}
		}

		if (similar.Num() > 1) 
		{
			OutError = shortOpt + TEXT(" is specified ambiguously ") + FString::FromInt(similar.Num()) + TEXT(" times.");
			return PatternList();
		} 
		else if (similar.Num()==0) 
		{
			UOption* newOpt = NewObject<UOption>();
			newOpt->ShortOption = shortOpt;
			newOpt->ArgumentCount=0;
			OutOptions.Add(newOpt);

			if (InTokens->IsParsingArgv) 
			{
				newOpt->SetValue(true);
			}
			ret.Add(newOpt);
		} 
		else 
		{
			auto o = similar[0];
			UDocoptValue* val = NewObject<UDocoptValue>();
			if (o->ArgumentCount) 
			{
				if (i == token.GetCharArray().Num()) 
				{
					// consume the next token
					auto ttoken = InTokens->Current();
					if (ttoken.IsEmpty() || ttoken==TEXT("--")) 
					{
						OutError = shortOpt + TEXT(" requires an argument");
						return PatternList();
					}
					val->SetValue(InTokens->Pop());
				} 
				else 
				{
					// consume all the rest
					val->SetValue(FString::Chr(token[i]) + token.Right(i));
					i = token.GetCharArray().Num();
				}
			}

			if (InTokens->IsParsingArgv) 
			{
				if(val->IsValueValid())
				{
					o->SetValue(val);
				}
				else
				{
					o->SetValue(true);
				}
			}
			ret.Add(o);
		}
	}

	return ret;
}

PatternList UDocoptUsageParser::ParseLong(UTokens* InTokens, TArray<UOption*>& OutOptions, FString& OutError)
{
	// long ::= '--' chars [ ( ' ' | '=' ) chars ] ;
	FString longOpt, equal;
	UDocoptValue* val = NewObject<UDocoptValue>();
	
	FStringPartition part = UDocoptUtils::Partition(InTokens->Pop(), TEXT("="));
	longOpt = part.First;
	equal = part.Second;
	val->SetValue(part.Third);

	check(UDocoptUtils::StartsWith(longOpt, TEXT("--")));

	if (equal.IsEmpty()) 
	{
		val->SetEmpty();
	}

	// detect which options match this long option
	TArray<UOption*> similar;
	for(auto option : OutOptions) 
	{
		if (option->LongOption == longOpt)
		{
			similar.Add(option);
		}
	}

	// maybe allow similar options that match by prefix
	if (InTokens->IsParsingArgv && similar.Num() == 0) 
	{
		for(auto option : OutOptions) 
		{
			if (option->LongOption.IsEmpty())
			{
				continue;
			}
			if (UDocoptUtils::StartsWith(option->LongOption, longOpt))
			{
				similar.Add(option);
			}
		}
	}

	PatternList ret;

	if (similar.Num() > 1) 
	{ 
		// might be simply specified ambiguously 2+ times?
		TArray<FString> prefixes = LongOptions(similar);
		OutError = TEXT("'") + longOpt + TEXT("' is not a unique prefix: ");
		OutError.Append(UDocoptUtils::Join(prefixes, TEXT(", ")));
		return PatternList();
	}
	 else if (similar.Num()==0) 
	 {
		int argcount = equal.IsEmpty() ? 0 : 1;
		
		UOption* newOpt = NewObject<UOption>();
		newOpt->LongOption = longOpt;
		newOpt->ArgumentCount = argcount;
		OutOptions.Add(newOpt);

		if (InTokens->IsParsingArgv) 
		{
			if(argcount)
			{
				newOpt->SetValue(val);
			}
			else
			{
				newOpt->SetValue(true);
			}
		}
		ret.Add(newOpt);
	} 
	else 
	{
		auto o = similar[0];
		if (o->ArgumentCount == 0) 
		{
			if (val->IsValueValid())
			{
				OutError = o->LongOption + TEXT(" must not have an argument");
				return PatternList();
			}
		} 
		else 
		{
			if (!val->IsValueValid()) 
			{
				auto token = InTokens->Current();
				if (token.IsEmpty() || token == TEXT("--")) 
				{
					OutError = o->LongOption + TEXT(" requires an argument");
					return PatternList();
				}
				val->SetValue(InTokens->Pop());
			}
		}
		if (InTokens->IsParsingArgv) 
		{
			if(val->IsValueValid())
			{
				o->SetValue(val);
			}
			else
			{
				o->SetValue(true);
			}
		}
		ret.Add(o);
	}

	return ret;
}

TArray<FString> UDocoptUsageParser::LongOptions(TArray<UOption*> InOptions)
{
	TArray<FString> ret;
	for(auto opt : InOptions)
	{
		ret.Add(opt->LongOption);
	}
	return ret;
}

bool UDocoptUsageParser::IsArgumentSpec(FString InToken)
{
	if(InToken.IsEmpty())
	{
		return false;
	}

	if(InToken[0] == TEXT('<') && InToken[InToken.GetCharArray().Num() - 1] == TEXT('>'))
	{
		return true;
	}

	if(InToken == InToken.ToUpper())
	{
		return true;
	}

	return false;
}

TArray<FString> UDocoptUsageParser::ParseSection(FString InName, FString InSource)
{
	std::string first_pt{
			"(?:^|\\n)"  // anchored at a linebreak (or start of string)
			"("
		   "[^\\n]*"
	};
	std::string second_pt = TCHAR_TO_UTF8(*InName);
	std::string third_pt{
	    "[^\\n]*(?=\\n?)" // a line that contains the name
		   "(?:\\n[ \\t].*?(?=\\n|$))*"         // followed by any number of lines that are indented
		")"
	};

	std::regex const re_section_pattern {
		first_pt+second_pt+third_pt,
		std::regex::icase
	};

	std::string source = TCHAR_TO_UTF8(*InSource);

	TArray<FString> ret;
	std::for_each(std::sregex_iterator(source.begin(), source.end(), re_section_pattern),
		      std::sregex_iterator(),
		      [&](std::smatch const& match)
	{
		ret.Add(UDocoptUtils::Trim(FString(match[1].str().c_str())));
	});

	return ret;

}

template <typename T>
TArray<T*> UDocoptUsageParser::FlatFilter(UPattern* InPattern)
{
	TArray<UPattern*> Flattened = InPattern->Flatten([](UPattern const* p) -> bool
	{
		return Cast<T>(p);
	});

	TArray<T*> ret;
	for(auto p : Flattened)
	{
		ret.Add(Cast<T>(p));
	}
	return ret;
}