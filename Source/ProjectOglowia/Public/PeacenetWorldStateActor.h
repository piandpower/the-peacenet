// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UDesktopWidget.h"
#include "FManPage.h"
#include "EGovernmentAlertStatus.h"
#include "UPeacenetSaveGame.h"
#include "FGovernmentAlertInfo.h"
#include "UGameTypeAsset.h"
#include "PeacenetWorldStateActor.generated.h"

class USystemContext;
class UWallpaperAsset;
class UComputerService;
class UMarkovTrainingDataAsset;
class UProceduralGenerationEngine;
class UPeacegateProgramAsset;
class UTerminalCommand;
class UCommandInfo;
class UWindow;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerSystemContextReadyEvent, USystemContext*, InSystemContext);

UCLASS()
class PROJECTOGLOWIA_API APeacenetWorldStateActor : public AActor
{
	GENERATED_BODY()
	
public: // Constructors
	// Sets default values for this actor's properties
	APeacenetWorldStateActor();

private: // Properties
	UPROPERTY()
	TArray<UComputerService*> ComputerServices;

	UPROPERTY()
	UProceduralGenerationEngine* Procgen;

	UPROPERTY()
	TArray<USystemContext*> SystemContexts;

	UPROPERTY()
	TMap<int, FGovernmentAlertInfo> GovernmentAlertInfo;

public: //Properties
	UPROPERTY()
	TArray<UMarkovTrainingDataAsset*> MarkovData;

	UPROPERTY()
	TArray<UWallpaperAsset*> Wallpapers;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(ExposeOnSpawn))
	TSubclassOf<UDesktopWidget> DesktopClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn))
	UPeacenetGameTypeAsset* GameType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn))
	TSubclassOf<UWindow> WindowClass;

	UPROPERTY(BlueprintAssignable, Category = "Peacenet")
	FPlayerSystemContextReadyEvent PlayerSystemReady;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn))
	FString WorldSlot;

	UPROPERTY()
	TMap<FName, FManPage> ManPages;

	UPROPERTY()
	UPeacenetSaveGame* SaveGame;

	UPROPERTY()
	TArray<UPeacegateProgramAsset*> Programs;

	UPROPERTY()
	TMap<FName, UCommandInfo*> CommandInfo;

private: // Functions
	UFUNCTION()
	void LoadTerminalCommands();

	template<typename AssetType>
	bool LoadAssets(FName ClassName, TArray<AssetType*>& OutArray);

public:	// Functions
	FText GetTimeOfDay();

	UFUNCTION()
	TArray<UComputerService*> GetServicesFor(EComputerType InComputerType);

	UFUNCTION()
	void SaveWorld();

	UFUNCTION()
	FGovernmentAlertInfo GetAlertInfo(int InCharacterID);

	UFUNCTION(BlueprintCallable, Category = "Peacenet")
	void StartGame(TSubclassOf<UDesktopWidget> InDesktopClass, TSubclassOf<UWindow> InWindowClass);

	UFUNCTION()
	bool FindProgramByName(FName InName, UPeacegateProgramAsset*& OutProgram);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Network Map")
	bool ScanForServices(FString InIPAddress, TArray<FFirewallRule>& OutRules);

protected: // AActor overrides
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type InReason) override;
	
public:	// AActor Overrides
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
public: // Static functions
	// Used by the Ubiquity menu to see if the "Boot existing OS" screen should show.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Peacegate")
	static bool HasExistingOS();

	UFUNCTION(BlueprintCallable, Category = "Peacegate")
	static APeacenetWorldStateActor* LoadExistingOS(const APlayerController* InPlayerController);
};

