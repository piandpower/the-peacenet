// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UWorldGenerator.h"

int32 UWorldGenerator::GetSeedFromString(const FString& InSeedString)
{
	return 0;
}

FRandomStream UWorldGenerator::GetRandomNumberGenerator(int32 InSeed)
{
	return FRandomStream(InSeed);
}

void UWorldGenerator::GenerateCharacters(UPARAM(Ref) TArray<FPeacenetIdentity> CharacterArray, UPARAM(Ref) TArray<FComputer> ComputerArray, const FRandomStream& InGenerator, int32 InCharacterCount)
{

}