// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UWorldGenerator.h"

FString UWorldGenerator::GenerateRandomName(const FRandomStream& InGenerator, const TArray<FString> InFirstNames, TArray<FString> InLastNames)
{
	FString first = InFirstNames[InGenerator.RandRange(0, InFirstNames.Num() - 1)];
	FString last = InLastNames[InGenerator.RandRange(0, InLastNames.Num() - 1)];

	return first + TEXT(" ") + last;
}

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
		uint8 c = (uint8)Chars[i];
		result ^= c << (((i + 1) & 3) << 3);
	}

	return result;
}

FRandomStream UWorldGenerator::GetRandomNumberGenerator(int32 InSeed)
{
	return FRandomStream(InSeed);
}

void UWorldGenerator::GenerateCharacters(UPARAM(Ref) TArray<FPeacenetIdentity> CharacterArray, UPARAM(Ref) TArray<FComputer> ComputerArray, const FRandomStream& InGenerator, int32 InCharacterCount, const TArray<FString> InFirstNames, TArray<FString> InLastNames)
{
	int CountryCount = (int)ECountry::Num_Countries;

	for (int i = 0; i < CharacterArray.Num(); i++)
	{
		// Create new computer and identity
		FComputer PersonalComputer;
		FPeacenetIdentity Identity;

		
	}
}