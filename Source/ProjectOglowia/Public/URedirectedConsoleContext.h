// Copyright (c) 2018 Alkaline Thunder & The Peacenet

#pragma once

#include "CoreMinimal.h"
#include "UPiperContext.h"
#include "URedirectedConsoleContext.generated.h"

UCLASS()
class PROJECTOGLOWIA_API URedirectedConsoleContext : public UPiperContext
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString OutputFilePath;

	UPROPERTY()
	bool Overwrite = false;

	UFUNCTION(BlueprintCallable, Category = "Console Redirection")
	void DumpToFile(UConsoleContext* InConsole);
};
