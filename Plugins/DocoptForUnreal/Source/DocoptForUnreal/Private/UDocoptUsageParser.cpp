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