// Copyright (c) 2018 Alkaline Thunder & Bit Phoenix Software

#pragma once

#include "CoreMinimal.h"
#include "FManPage.generated.h"

USTRUCT()
struct FManPage
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FString Description;

	UPROPERTY()
	FString LongDescription;

	UPROPERTY()
	FString InternalUsage;

	UPROPERTY()
	FString FriendlyUsage;
};