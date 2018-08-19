// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UVirtualMachineService.h"
#include "AssetRegistryModule.h"
#include "VirtualMachine.generated.h"

UENUM(BlueprintType)
enum class ECountry : uint8
{
	Elytrose,
	Oglowia,
	Sikkim,
	Velacrol,
	Riogan,
	Valkerie,
	Mejionde
};

USTRUCT(BlueprintType)
struct FMachineInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Hostname;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName MachineType;
};

USTRUCT(BlueprintType)
struct FCharacterSpawnInfo {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Name;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int XP;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ECountry Country;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FMachineInfo> OwnedMachines;
};

USTRUCT(BlueprintType)
struct FPeacenetCampaignInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Name;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Description;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString WorldSeed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool DoTutorial;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FCharacterSpawnInfo> StoryCharacters;
};


UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UPeacenetCampaignAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Peacenet Campaign")
	FPeacenetCampaignInfo CampaignInformation;

};

UCLASS()
class PROJECTOGLOWIA_API AVirtualMachine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVirtualMachine();

	UPROPERTY(BlueprintReadOnly)
	FString Hostname;

	void SpinUpSystem(const FMachineInfo& InMachineInfo);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	TArray<UVirtualMachineService*> Services;
	
};