// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TutorialBase.generated.h"

UCLASS()
class PROJECTOGLOWIA_API ATutorialBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATutorialBase();

	UFUNCTION(BlueprintCallable, Category = "Peacenet Tutorial")
	bool IsTutorialCompleted();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category="Peacenet Tutorial")
	void CompleteTutorial();

private:
	bool bTutorialCompleted = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
