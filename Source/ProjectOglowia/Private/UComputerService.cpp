// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UComputerService.h"

bool UComputerService::CanHack(USystemContext * InCaller)
{
	check(HackableType); //Just gonna crash dev builds when the hackable type isn't specified because that's fucking stupid.

	if (!this->HackableType)
		return false; //What the fuck? There's no hackable type?

	return this->HackableType->CanHack(InCaller); // we can hack if the hackable says we can.
}
