// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UWorldGenerator.h"

int32 UWorldGenerator::GetSeedFromString(const FString& InSeedString)
{
	if (InSeedString.IsNumeric())
	{
		return FCString::Atoi(*InSeedString);
	}

	if (InSeedString.IsEmpty())
		return 8675309;

	TArray<TCHAR> Chars = InSeedString.GetCharArray();

	// Below algorithm is written by Declan Hoare (NetwideRogue).

	int result = 0;
	int charCount = Chars.Num();

	for (int i = 0; i < charCount; i++)
	{
		uint8 c = (uint8)Chars[charCount];
		result ^= c << (((i + 1) & 3) << 3);
	}

	return result;
}

FRandomStream UWorldGenerator::GetRandomNumberGenerator(int32 InSeed)
{
	return FRandomStream(InSeed);
}

void UWorldGenerator::GenerateCharacters(UPARAM(Ref) TArray<FPeacenetIdentity> CharacterArray, UPARAM(Ref) TArray<FComputer> ComputerArray, const FRandomStream& InGenerator, int32 InCharacterCount)
{

}