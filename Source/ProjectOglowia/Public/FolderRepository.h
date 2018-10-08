// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "FFolder.h"
#include "UObject/Interface.h"
#include "FolderRepository.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UFolderRepository : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTOGLOWIA_API IFolderRepository
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Save System")
	void GetFolderTree(TArray<FFolder>& OutFolderTree);

	UFUNCTION(BlueprintImplementableEvent, Category = "Save System")
	void PushFolderTree(const TArray<FFolder>& InFolderTree);
};
