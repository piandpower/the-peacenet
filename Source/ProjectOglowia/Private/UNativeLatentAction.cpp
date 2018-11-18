// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UNativeLatentAction.h"

bool UNativeLatentAction::Tick(float InDeltaTime)
{
	bool result = this->OnTick(InDeltaTime);
	if (result)
		this->OnDone.Broadcast();
	return result;
}

bool UNativeLatentAction::OnTick(float InDeltaTime)
{
	return true;
}