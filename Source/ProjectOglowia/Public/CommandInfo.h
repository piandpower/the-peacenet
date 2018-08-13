// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "TerminalCommand.h"
#include "CommandInfo.generated.h"

USTRUCT(BlueprintType)
struct PROJECTOGLOWIA_API FCommandInfoS
{
	GENERATED_BODY()

		// can either be an internal name or the actual text the player would enter to run the command
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName CommandName;

	// should be the class of command which the player runs, given the command name.
	UPROPERTY(EditAnywhere, BlueprintReadWrite) //@note(lambda): MustImplement must be the same as your interface object name in the content browser - C++ interfaces drop the prefix 'I' or 'U'.
		TSubclassOf<UTerminalCommand> CommandClass;
};

UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UCommandInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FCommandInfoS Info;
};