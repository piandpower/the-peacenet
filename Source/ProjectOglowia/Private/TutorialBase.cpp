// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "TutorialBase.h"


// Sets default values
ATutorialBase::ATutorialBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

bool ATutorialBase::IsTutorialCompleted()
{
	return bTutorialCompleted;
}

// Called when the game starts or when spawned
void ATutorialBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATutorialBase::CompleteTutorial()
{
	bTutorialCompleted = true;
}

// Called every frame
void ATutorialBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

