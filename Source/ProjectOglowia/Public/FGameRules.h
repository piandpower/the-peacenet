// Copyright (c) 2018 Alkaline Thunder & Bit Phoenix Software

#pragma once

#include "CoreMinimal.h"
#include "ECountry.h"
#include "FGameRules.generated.h"

USTRUCT(BlueprintType)
struct PROJECTOGLOWIA_API FGameRules
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Rules")
    bool DoUnlockables = true;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Rules")
    bool DoMissions = true;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Rules")
    bool DoTutorial = true;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Rules")
    bool DoTimeOfDay = true;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Rules")
    bool DoSideMissions = true;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Rules")
    bool ConsiderReputations = true;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Rules")
    bool DoHackerAI = true;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Rules")
    bool DoGovernmentAlert = true;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Rules")
    bool DoFirewalls = true;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Rules")
    bool DoSkillProgression = true;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Rules")
	ECountry SpawnCountry = ECountry::Oglowia;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Rules")
    int MaximumSkillLevel = 15;
};