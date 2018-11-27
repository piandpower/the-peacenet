// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UHackableHandler.h"

bool UHackableHandler::CanHandle(USystemContext* InCaller)
{
	return false;
}

void UHackableHandler::Handle(UVulnerability* InVulnerability, USystemContext* InCaller, USystemContext* InTarget, int InUid)
{

}