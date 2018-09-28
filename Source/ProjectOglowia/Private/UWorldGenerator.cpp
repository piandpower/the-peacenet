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

FString UWorldGenerator::GenerateRandomPassword(const FRandomStream& InGenerator, int InLength)
{
	FString NewPassword;

	const FString Chars = TEXT("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890!@#$%^&*()_+-=`~]}[{'\"\\|/?.>,<");

	TArray<TCHAR> CharArr = Chars.GetCharArray();

	for (int i = 0; i < InLength; i++)
	{
		TCHAR Char = CharArr[InGenerator.RandRange(0, CharArr.Num() - 1)];

		NewPassword = NewPassword.AppendChar(Char);
	}

	return NewPassword;
}

FRandomStream UWorldGenerator::GetRandomNumberGenerator(int32 InSeed)
{
	return FRandomStream(InSeed);
}

void UWorldGenerator::GenerateCharacters(UPARAM(Ref) TArray<FPeacenetIdentity>& CharacterArray, UPARAM(Ref) TArray<FComputer>& ComputerArray, const FRandomStream& InGenerator, int32 InCharacterCount, const TArray<FString> InFirstNames, TArray<FString> InLastNames)
{
	int CountryCount = (int)ECountry::Num_Countries;

	for (int i = 0; i < InCharacterCount; i++)
	{
		// Create new computer and identity
		FComputer PersonalComputer;
		FPeacenetIdentity Identity;

		// Set the entity IDs of the computer and character.
		PersonalComputer.ID = ComputerArray.Num();
		Identity.ID = CharacterArray.Num();

		// These are all NPCs.
		Identity.CharacterType = EIdentityType::NonPlayer;

		// Set default alert stuff
		Identity.AlertLevel = 0.f;
		Identity.ActiveGovernmentWatch = false;
		Identity.Skill = 0;

		uint8 Country = (uint8)InGenerator.RandRange(0, CountryCount - 1);
		Identity.Country = (ECountry)Country;

		// Set the reputation
		int RepDiceRoll = InGenerator.RandRange(1, 6);
		if (RepDiceRoll % 2 == 0)
		{
			Identity.Reputation = InGenerator.GetFraction();
		}
		else
		{
			Identity.Reputation = -InGenerator.GetFraction();
		}

		// Get a random number between 1 and 256 to decide if we should make a person NPC or a business NPC
		int businessDecider = InGenerator.RandRange(1, 256);

		if (businessDecider % 2 == 0 && businessDecider % 3 == 0)
		{
			// Make a business.
		}
		else
		{
			// We'll make a person.
			FString CharName = UWorldGenerator::GenerateRandomName(InGenerator, InFirstNames, InLastNames);

			FString First;
			FString Last;

			TArray<FString> Split;

			CharName.ParseIntoArray(Split, TEXT(" "), true);

			First = Split[0];
			Last = Split[1];

			Identity.CharacterName = FText::FromString(CharName);

			PersonalComputer.Hostname = FText::FromString(First.ToLower() + TEXT("-pc"));

			FUser RootUser;
			RootUser.ID = 0;
			RootUser.Username = FText::FromString(TEXT("root"));
			RootUser.Password = FText::FromString(UWorldGenerator::GenerateRandomPassword(InGenerator, 16));
			RootUser.Domain = EUserDomain::Administrator;

			FUser PowerUser;
			PowerUser.ID = 1;
			PowerUser.Username = FText::FromString(First.ToLower());
			PowerUser.Password = FText::FromString(UWorldGenerator::GenerateRandomPassword(InGenerator, 16));
			PowerUser.Domain = EUserDomain::PowerUser;

			PersonalComputer.Users.Add(RootUser);
			PersonalComputer.Users.Add(PowerUser);

			PersonalComputer.ComputerType = EComputerType::PersonalComputer;
		}

		Identity.PersonalComputer = PersonalComputer.ID;

		CharacterArray.Add(Identity);
		ComputerArray.Add(PersonalComputer);
	}
}