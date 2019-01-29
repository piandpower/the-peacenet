// Copyright (c) 2019 Alkaline Thunder & Bit Phoenix Software.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "Engine/DataAsset.h"
#include "EComputerType.h"
#include "UHackable.h"
#include "UComputerService.generated.h"

UENUM(BlueprintType)
enum class EAuthenticationType : uint8
{
    None,
    Basic,
    Credential,
    Crypto
};

UCLASS(BlueprintType, Blueprintable)
class PROJECTOGLOWIA_API UComputerService : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Metadata")
    FText Name;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Metadata")
    FText Description;
    
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Metadata")
    FText LongDescription;
    
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Metadata")
    UTexture2D* Icon;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Authentication")
    EAuthenticationType AuthenticationType = EAuthenticationType::None;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Spawning")
    EComputerType TargetComputerType = EComputerType::Personal;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Spawning")
    bool IsDefault = false;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Spawning")
    int Port;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Behaviour")
    TSubclassOf<UHackable> Hackable;

};