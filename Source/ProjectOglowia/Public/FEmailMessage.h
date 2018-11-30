// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "Text.h"
#include "FEmailAttachment.h"
#include "FEmailMessage.generated.h"

/**
 * Represents a mission sent with an email.
 */
USTRUCT(BlueprintType)
struct PROJECTOGLOWIA_API FEmailMission
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "Email|Mission")
	FName MissionID;
};
/**
 * Represents an email message.
 */
USTRUCT(BlueprintType)
struct PROJECTOGLOWIA_API FEmailMessage
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Email")
	int EntityID = 0;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Email")
	FText Subject;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Email")
	FText Message;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Email")
	TArray<FEmailAttachment> Attachments;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Email")
	TArray<FEmailMission> Missions;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Email")
	int From = 0;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Email")
	int To = 0;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Email")
	int InReplyTo = 0;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Email")
	bool IsUnread = true;	
};
