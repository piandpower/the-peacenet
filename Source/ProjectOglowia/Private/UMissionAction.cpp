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

void ULatentMissionAction::Tick(float InDeltaTime)
{
	this->NativeTick(InDeltaTime);
	this->K2_Tick(InDeltaTime);
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

void UMissionAction::ExecuteMissionAction()
{
	this->NativeExecuteMissionAction();
	this->K2_ExecuteMissionAction();
}
