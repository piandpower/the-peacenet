// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "UEmailAttachmentAsset.h"
#include "FEmailAttachment.generated.h"

/**
 * Represents an email attachment.
 */
USTRUCT(BlueprintType)
struct PROJECTOGLOWIA_API FEmailAttachment
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Email|Attachment")
	FString Name;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Email|Attachment")
	UEmailAttachmentAsset* Data;
};
