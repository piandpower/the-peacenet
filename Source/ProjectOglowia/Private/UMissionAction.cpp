// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UMissionAction.h"

bool ULatentMissionAction::IsCompleted()
{
	return this->bIsActionFinished;
}

bool ULatentMissionAction::IsFailed()
{
	return this->bIsActionFailed;
}

FString ULatentMissionAction::GetFailReasonText()
{
	return this->FailReason;
}

void ULatentMissionAction::Complete()
{
	this->bIsActionFinished = true;
}

void ULatentMissionAction::Fail(const FString& InFailReason)
{
	this->bIsActionFailed = true;
	this->FailReason = InFailReason;
}
