// Copyright (c) 2019 Alkaline Thunder & Bit Phoenix Software

#pragma once

#include "CoreMinimal.h"
#include "LatentActions.h"

class FPlaceboLatentAction : public FPendingLatentAction
{
public:
	FName ExecutionFunction;
	int32 OutputLink;
	FWeakObjectPtr CallbackTarget;

	FPlaceboLatentAction(const FLatentActionInfo& InLatentInfo)
		: ExecutionFunction(InLatentInfo.ExecutionFunction)
		, OutputLink(InLatentInfo.Linkage)
		, CallbackTarget(InLatentInfo.CallbackTarget) {}

	virtual void UpdateOperation(FLatentResponse& Response) override
	{
		Response.DoneIf(true);
	}
};
