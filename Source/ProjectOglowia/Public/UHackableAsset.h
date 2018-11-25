// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/Texture2D.h"

#include "UHackableAsset.generated.h"

class USystemContext;
class UHackableHandler;

/**
 * Represents a hackable service within The Peacenet.
 */
UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UHackableAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName InternalID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText DisplayName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Port;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTexture2D* Icon;

	// Specifies a class that will be used to handle a successful hack of services that implement this hackable type.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Instanced, Category = "Gameplay")
	UHackableHandler* Handler;

	UFUNCTION()
	bool CanHack(USystemContext* InCaller);
};