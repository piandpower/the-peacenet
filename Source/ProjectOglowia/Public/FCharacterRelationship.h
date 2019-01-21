// Copyright (c) 2019 Alkaline Thunder & Bit Phoenix Software

#pragma once

#include "CoreMinimal.h"
#include "FCharacterRelationship.generated.h"

UENUM(BlueprintType)
enum class ERelationshipType : uint8
{
    Friend,
    Enemy
};

USTRUCT(BlueprintType)
struct PROJECTOGLOWIA_API FCharacterRelationship
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Relationships")
    int FirstEntityID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Relationships")
    int SecondEntityID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Relationships")
    ERelationshipType RelationshipType;
};