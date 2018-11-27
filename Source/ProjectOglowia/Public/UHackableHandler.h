// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "UVulnerability.h"
#include "USystemContext.h"
#include "UHackableHandler.generated.h"

/**
 * A handler class that is used to decide what happens when you successfully gain access to a computer service.
 */
UCLASS(BlueprintType, Abstract, EditInlineNew)
class PROJECTOGLOWIA_API UHackableHandler : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual bool CanHandle(USystemContext* InCaller);

	UFUNCTION()
	virtual void Handle(UVulnerability* InVulnerability, USystemContext* InCaller, USystemContext* InTarget, int InUid);
};
