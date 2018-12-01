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
	UPROPERTY(BlueprintReadOnly, Category = "Character Display")
	FPeacenetIdentity Character;

	UPROPERTY(BlueprintReadOnly, Category = "Character Display")
	FComputer Computer;

	UPROPERTY(BlueprintReadOnly, Category = "Character Display")
	bool IsPinned = false;

	UFUNCTION(BlueprintCallable, Category = "Character Display")
	void SetCharacterIdChecked(int InEntityID);



public: //overrides
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected: // functions that only we have access to.

	UFUNCTION(BlueprintCallable, Category = "Character Display")
	void PinContactChecked();
};
