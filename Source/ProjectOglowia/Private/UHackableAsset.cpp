// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UHackableAsset.h"
#include "USystemContext.h"
#include "UHackableHandler.h"

bool UHackableAsset::CanHack(USystemContext * InCaller)
{
	if(!this->Handler) // You can't hack this hackable if there's no handler at all.
		return false;

	return this->Handler->CanHandle(InCaller); // Can the handler be used to handle a hack from this system?
}
