// Copyright (c) 2018 The Peacenet & Alkaline Thunder.


#include "UDocoptUsageParser.h"
#include "UDocoptUtils.h"

UTokens* UTokens::FromPattern(FString InSource)
{
    // TODO: Actually do stuff.
    UTokens* NewTokens = NewObject<UTokens>();
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