// Copyright (c) 2018 Alkaline Thunder & The Peacenet

#pragma once

#include "CoreMinimal.h"
#include "Text.h"
#include "FContactNote.generated.h"

/**
 * Represents a user-made note on a contact in the Peacenet Address Book.
 */
USTRUCT(BlueprintType)
struct PROJECTOGLOWIA_API FContactNote
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Contact|User Note")
    FText NoteMessage;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Contact|User Note")
    float Timestamp = 0.f;
};