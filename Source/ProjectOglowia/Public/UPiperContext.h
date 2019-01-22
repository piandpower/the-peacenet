// Copyright (c) 2018 Alkaline Thunder & The Peacenet.

#pragma once

#include "CoreMinimal.h"
#include "UConsoleContext.h"
#include "FPlaceboLatentAction.h"
#include "UPiperContext.generated.h"

/**
 * Encapsulates a console context which acts as a pipe between two other consoles.
 */
UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UPiperContext : public UConsoleContext
{
	GENERATED_BODY()

private:
    UPROPERTY()
	UPiperContext* Input;

    UPROPERTY()
	UConsoleContext* Output;

    UPROPERTY()
	FString Log;

public:	
    UFUNCTION()
    void SetupPiper(UPiperContext* InInput, UConsoleContext* InOutput);

    UFUNCTION()
    FString GetLog();

	virtual void Write(const FString& InText) override;
	virtual void WriteLine(const FString& InText) override;
	virtual void OverwriteLine(const FString& InText) override;
	virtual UConsoleContext* CreateChildContext(USystemContext* InSystemContext, int InUserID) override;
	virtual void Clear() override;
	virtual FString SynchronouslyReadLine() override;

	FString GetInputBuffer();

	virtual void ReadLine(UObject* WorldContextObject, struct FLatentActionInfo LatentInfo, FString& OutText) override;
};
