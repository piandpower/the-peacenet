// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UCompanyTypeAsset.h"
#include "UWorldGenerator.h"

void UCompanyTypeAsset::GenerateName(const FRandomStream& InRandomStream, TArray<FString> BaseTrainingData, FString & OutRealName, FString & OutDomainName)
{
	// We add our training data into the base data to get company names that match our type.
	BaseTrainingData.Append(this->NameTrainingData->TrainingData);

	// Now we create a new markov chain.
	UMarkovChain* Markov = UWorldGenerator::CreateMarkovChain(BaseTrainingData, InRandomStream);

	// Generate the real name.
	OutRealName = Markov->GetMarkovString(0);

	// Domain name is a stripped version of the real name.
	FString KeptChars = "abcdefghijklmnopqrstuvwxyz1234567890-_";

	// convert it to lowercase
	OutDomainName = OutRealName.ToLower();

	for (int i = 0; i < OutDomainName.GetCharArray().Num(); i++)
	{
		if (!KeptChars.Contains(FString::Chr(OutDomainName[i])))
		{
			OutDomainName.RemoveAt(i, 1);
			i--;
		}
	}

	TArray<FString> TopLevelDomains = { ".com", ".net", ".org" };
	OutDomainName = OutDomainName.Append(TopLevelDomains[InRandomStream.RandRange(0, TopLevelDomains.Num() - 1)]);
}
