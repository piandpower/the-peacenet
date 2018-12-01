 // Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "FComputer.h"
#include "FPeacenetIdentity.h"
#include "UPeacegateProgramAsset.h"
#include "UCharacterDisplayBase.generated.h"

/**
 * A base user widget for character display programs.
 */
UCLASS(Blueprintable, BlueprintType, Abstract)
class PROJECTOGLOWIA_API UCharacterDisplayBase : public UProgram
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Character Display", meta = (ExposeOnSpawn))
	FPeacenetIdentity Character;

	UFUNCTION(BlueprintCallable, Category = "Character Display")
	void SetCharacterIdChecked(int InEntityID);
};
