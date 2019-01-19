#include "UMarkovChain.h"

TCHAR UMarkovChain::GetNext(FMarkovSource InSource)
{
	if (!MarkovMap.Contains(InSource))
		return TEXT('\0');

	TMap<TCHAR, int> Map = this->MarkovMap[InSource];

	int Total = 0;

	TArray<TCHAR> Keys;

	int KeyCount = Map.GetKeys(Keys);

	for (int i = 0; i < KeyCount; i++)
	{
		Total += Map[Keys[i]];
	}

	int Rng = this->Random.RandRange(0, Total - 1);

	for (int i = 0; i < KeyCount; i++)
	{
		Rng -= Map[Keys[i]];
		if (Rng < 0)
		{
			return Keys[i];
		}
	}

	return TEXT('\0');
}

FMarkovSource UMarkovChain::RandomSource()
{
	TArray<FMarkovSource> Keys;

	int KeyCount = this->MarkovMap.GetKeys(Keys);

	return Keys[Random.RandRange(0, KeyCount - 1)];
}

bool UMarkovChain::IsDeadEnd(FMarkovSource InSource, int Depth)
{
	if (Depth <= 0)
		return false;

	if (!MarkovMap.Contains(InSource))
		return true;

	TMap<TCHAR, int> Map = this->MarkovMap[InSource];

	TArray<TCHAR> Keys;

	if (Map.GetKeys(Keys) == 1)
		if(Keys[0] == TEXT('\0'))
			return true;

	FMarkovSource TempSource = InSource;

	for (int i = 0; i < Keys.Num(); ++i)
	{
		TempSource = InSource;
		TempSource.Rotate(Keys[i]);
		if (!IsDeadEnd(TempSource, Depth - 1)) return false;
	}

	return true;
}

TCHAR UMarkovChain::GetNextCharGuarantee(FMarkovSource InSource, int InSteps)
{
	check(!IsDeadEnd(InSource, InSteps));

	TMap<TCHAR, int> Temp;
	TMap<TCHAR, int> Map = MarkovMap[InSource];

	TArray<TCHAR> Keys;

	int KeyCount = Map.GetKeys(Keys);

	if (KeyCount == 1)
		return Keys[0];

	check(KeyCount > 0);

	int Total = 0;
	for (int i = 0; i < KeyCount; ++i)
	{
		FMarkovSource TempSource = InSource;
		TempSource.Rotate(Keys[i]);
		if (!IsDeadEnd(TempSource, InSteps))
		{
			if (Temp.Contains(Keys[i]))
				Temp[Keys[i]] = Map[Keys[i]];
			else
				Temp.Add(Keys[i], Map[Keys[i]]);
			Total += Map[Keys[i]];
		}
	}

	int Rng = Random.RandHelper(Total);

	TArray<TCHAR> TempKeys;

	int TempKeyCount = Temp.GetKeys(TempKeys);

	for (int i = 0; i < TempKeyCount; i++)
	{
		Rng -= Temp[TempKeys[i]];
		if (Rng < 0)
		{
			return TempKeys[i];
		}
	}

	check(Rng < 0);

	return TEXT('\0');
}

void UMarkovChain::Init(TArray<FString> InArray, int N, FRandomStream InRng)
{
	this->Random = InRng;

	for (FString ArrayString : InArray)
	{
		FMarkovSource Source;
		Source.SetCount(N);

		for (TCHAR Char : ArrayString.ToLower())
		{
			if (Char == TEXT('\0'))
				break;

			if (!MarkovMap.Contains(Source))
				MarkovMap.Add(Source, TMap<TCHAR, int>());

			if (!MarkovMap[Source].Contains(Char))
				MarkovMap[Source].Add(Char, 0);

			MarkovMap[Source][Char]++;
			Source.Rotate(Char);
		}
		if (!MarkovMap.Contains(Source))
			MarkovMap.Add(Source, TMap<TCHAR, int>());

		if (!MarkovMap[Source].Contains(TEXT('\0')))
			MarkovMap[Source].Add(TEXT('\0'), 0);

		MarkovMap[Source][TEXT('\0')]++;
	}

	SourceCount = N;
}

FString UMarkovChain::GetMarkovString(int InLength)
{
	FString Out;

	FMarkovSource src;
	src.SetCount(SourceCount);

	if (InLength < 1) 
	{
		TCHAR tmp = GetNext(src);
		while (tmp != TEXT('\0'))
		{
			Out.AppendChar(tmp);
			src.Rotate(tmp);
			tmp = GetNext(src);
		}
		return Out;
	}
	for (int i = 0; i<InLength; ++i) {
		int x = (i > InLength - SourceCount ? InLength - i : SourceCount);
		TCHAR tmp = GetNextCharGuarantee(src, x);
		Out.AppendChar(tmp);
		src.Rotate(tmp);
	}
	return Out;
}