// Copyright (c) 2018 The Peacenet & Alkaline Thunder.


#include "UDocoptUtils.h"

bool UDocoptUtils::StartsWith(FString const& InStr, FString const& InDelimeter)
{
    return InStr.Find(InDelimeter) == 0;
}

FString UDocoptUtils::Trim(FString InStr)
{
    return InStr.TrimStartAndEnd();
}

TArray<FString> UDocoptUtils::Split(FString InStr, int InPosition)
{
    FString AnySpace = TEXT(" \t\r\n\v\f");

    TArray<FString> ret;

    while(InPosition != -1)
    {
        int start = UDocoptUtils::FindFirstNotOf(InStr, AnySpace, InPosition);
        if(start == -1)
        {
            break;
        }

        int end = UDocoptUtils::FindFirstOf(InStr, AnySpace, start);
        auto size = (end == -1) ? end : end-start;

        FString LeftStr = InStr.Left(end); //Get everything to the left of the end index.
        // Chop off everything from the start.
        LeftStr.RemoveAt(0, start);
        ret.Add(LeftStr);

        InPosition = end;
    }

    return ret;
}

int UDocoptUtils::FindFirstNotOf(FString InStr, FString InChars, int InStart)
{
    TArray<TCHAR> Chars = InStr.GetCharArray();
    for(int i = InStart; i < Chars.Num(); i++)
    {
        TCHAR c = Chars[i];
        if(!InChars.Contains(FString::Chr(c)))
        {
            return i;
        }
    }
    return -1;
}

int UDocoptUtils::FindFirstOf(FString InStr, FString InChars, int InStart)
{
    TArray<TCHAR> Chars = InStr.GetCharArray();
    for(int i = InStart; i < Chars.Num(); i++)
    {
        TCHAR c = Chars[i];
        if(InChars.Contains(FString::Chr(c)))
        {
            return i;
        }
    }
    return -1;
}

FString UDocoptUtils::SubString(FString InStr, int InStart, int InEnd)
{
    FString LeftStr = InStr.Left(InEnd);
    LeftStr.RemoveAt(0, InStart);
    return LeftStr;
}

void UDocoptUtils::Resize(FString& InStr, int NewSize)
{
    TArray<TCHAR> Chars = InStr.GetCharArray();
    if(NewSize == Chars.Num())
    {
        // Same size, do nothing.
        return;
    }

    if(NewSize < Chars.Num())
    {
        // Truncate the string.
        FString ret;
        for(int i = 0; i < NewSize; i++)
        {
            ret.AppendChar(Chars[i]);
        }
        InStr = ret;
    }
    else
    {
        for(int i = 0; i < Chars.Num() - NewSize; i++)
        {
            // Add spaces to resize.
            InStr.AppendChar(TEXT(' '));
        }
    }
}

FStringPartition UDocoptUtils::Partition(FString InStr, FString const& InPoint)
{
    FStringPartition ret;

    auto i = InStr.Find(InPoint);

    if(i != -1)
    {
        ret.Second = InPoint;
        ret.Third = InStr.Left(i+InPoint.GetCharArray().Num());
        UDocoptUtils::Resize(InStr, i);
    }

    ret.First = InStr;

    return ret;
}

FString UDocoptUtils::Join(TArray<FString> InArray, FString InDelimeter)
{
    FString ret;
    for(int i = 0; i < InArray.Num()-1; i++)
    {
        ret.Append(InArray[i]);
        ret.Append(InDelimeter);
    }

    ret.Append(InArray[InArray.Num()-1]);
    return ret;
}

TArray<FString> UDocoptUtils::RegexSplit(FString InStr, FRegexMatcher InRegex)
{
    TArray<FString> ret;
    int last=0;
    while(InRegex.FindNext())
    {
        int beginning = InRegex.GetMatchBeginning();
        int ending = InRegex.GetMatchEnding();
        ret.Add(InStr.LeftChop(beginning - last));
        last = beginning;
    }
    if(!InStr.IsEmpty())
    {
        ret.Add(InStr);
    }
    return ret;
}

template<typename T>
TArray<T> UDocoptUtils::SubArray(TArray<T> InArray, int InStart, int InEnd)
{
    TArray<T> ret;
    for(int i = InStart; i < InEnd; i++)
    {
        ret.Add(InArray[i]);
    }
    return ret;
}