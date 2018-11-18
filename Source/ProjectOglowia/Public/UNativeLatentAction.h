// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "UNativeLatentAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNativeLatentActionDone);

/**
 * A native latent action that is handled similarly to a Blueprint one but is done purely by Peacenet.
 */
UCLASS()
class PROJECTOGLOWIA_API UNativeLatentAction : public UObject
{
	GENERATED_BODY()

public:
	// Code that runs every frame. Return value is whether the action is finished.
	bool Tick(float InDeltaTime);

	FNativeLatentActionDone OnDone;

protected:
	virtual bool OnTick(float InDeltaTime);
};
