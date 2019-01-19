#include "FMarkovSource.h"

void FMarkovSource::SetCount(int InCount)
{
	this->Chars = TArray<TCHAR>();

	for (int i = 0; i < InCount; i++)
	{
		Chars.Add(TEXT('\0'));
	}
}

void FMarkovSource::Rotate(TCHAR NextChar)
{
	check(Chars.Num() > 0);

	for (int i = 0; i < Chars.Num() - 1; i++)
	{
		Chars[i] = Chars[i + 1];
	}
	Chars[Chars.Num() - 1] = NextChar;
	Data = ToString();
}

bool FMarkovSource::IsLessThan(const FMarkovSource& OtherSource)
{
	int i = 0;
	for (i = 0; i < Chars.Num() - 1; i++)
	{
		if (Chars[i] != OtherSource.Chars[i]) break;
	}
	return Chars[i] < OtherSource.Chars[i];
}

bool FMarkovSource::IsStartSource()
{
	for (auto Char : Chars)
	{
		if (Char != TEXT('\0')) return false;
	}
	return true;
}

bool FMarkovSource::operator==(const FMarkovSource & Other) const
{
	return Chars == Other.Chars;
}

FString FMarkovSource::ToString() const
{
    FString Ret;

    for(int i = 0; i < this->Chars.Num(); i++)
    {
        Ret.AppendChar(Chars[i]);
    }

    return Ret;
}

TArray<TCHAR> const& FMarkovSource::GetChars() const
{
    return this->Chars;
}