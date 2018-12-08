// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UDesktopWidget.h"
#include "UPeacenetSaveGame.h"
#include "UMissionAction.h"
#include "UGameTypeAsset.h"
#include "PeacenetWorldStateActor.generated.h"

class UNativeLatentAction;
class UMissionAsset;
class UComputerService;
class UComputerTypeAsset;
class USystemContext;
class UWallpaperAsset;
class UPeacegateProgramAsset;
class UTerminalCommand;
class UCommandInfo;
class UWindow;
class UVulnerability;
class UMissionUnlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerSystemContextReadyEvent, USystemContext*, InSystemContext);
DECLARE_DYNAMIC_DELEGATE_OneParam(FShowLoadingScreenEvent, UWorldGeneratorStatus*, InStatus);

USTRUCT(BlueprintType)
struct PROJECTOGLOWIA_API FPeacenetWorldInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString PlayerName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString PlayerUsername;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString PlayerHostname;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString PlayerPassword;
};

USTRUCT()
struct FManPage
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FString Description;

	UPROPERTY()
	FString LongDescription;

	UPROPERTY()
	FString InternalUsage;

	UPROPERTY()
	FString FriendlyUsage;
};

UCLASS()
class PROJECTOGLOWIA_API APeacenetWorldStateActor : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	TArray<UVulnerability*> Vulnerabilities;

public:
	UFUNCTION()
	bool FindServiceByName(FName ServiceName, UComputerService*& OutService);

private:
	UPROPERTY()
	TArray<UNativeLatentAction*> LatentActions;

	UPROPERTY()
	bool bIsMissionPaused = false;

	UPROPERTY()
	UPeacenetSaveGame* MissionStartSaveGame;

	UPROPERTY()
	UPeacenetSaveGame* CheckpointSaveGame;

public:
	UPROPERTY()
	TArray<UMissionUnlock*> MissionUnlocks;

private:
	UPROPERTY()
	TArray<UMissionUnlock*> CheckpointMissionUnlocks;

	UPROPERTY()
	UMissionAsset* CurrentMissionAsset;

	UFUNCTION()
	void LoadTerminalCommands();

	UPROPERTY()
	USystemContext* MissionContext;

	UPROPERTY()
	TArray<UMissionAction*> CurrentMissionActions;

	UPROPERTY()
	TArray<UMissionAction*> CheckpointMissionActions;

	UPROPERTY()
	ULatentMissionAction* CurrentLatentMissionAction;

	UFUNCTION()
	void CompleteMission();

	UFUNCTION()
	void ResynchronizeSystemContexts();

public:	
	UPROPERTY()
	TArray<UComputerService*> ComputerServices;

	UFUNCTION()
	void AddLatentAction(UNativeLatentAction* InAction);

	UFUNCTION()
	bool ResolveHost(FString InHost, FString& ResolvedIP, USystemContext*& ResolvedContext);

	UPROPERTY()
	TArray<UMissionAsset*> Missions;

	UFUNCTION()
	void SaveWorld();

	UPROPERTY()
	TArray<UWallpaperAsset*> Wallpapers;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(ExposeOnSpawn))
	TSubclassOf<UDesktopWidget> DesktopClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn))
	UPeacenetGameTypeAsset* GameType;

	UFUNCTION()
	bool UpdateComputer(int InEntityID, FComputer& InComputer, bool InSaveGame = true);
	
	UFUNCTION()
	bool UpdateCharacter(int InEntityID, FPeacenetIdentity& InCharacter);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn))
	TSubclassOf<UWindow> WindowClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn))
	UComputerTypeAsset* PlayerComputerType;

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

	// Sets default values for this actor's properties
	APeacenetWorldStateActor();

public:
	UFUNCTION()
	bool StartMission(UMissionAsset* InMission, USystemContext* InMissionSystem);

private:

	UPROPERTY()
	TArray<USystemContext*> SystemContexts;

	template<typename AssetType>
	bool LoadAssets(FName ClassName, TArray<AssetType*>& OutArray);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type InReason) override;
	
public:	
	UPROPERTY()
	TArray<UComputerTypeAsset*> ComputerTypes;

	UFUNCTION()
	bool FindProgramByName(FName InName, UPeacegateProgramAsset*& OutProgram);

	FText GetTimeOfDay();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Peacenet")
	void StartGame();
	
public:
	UPROPERTY()
	UWorldGeneratorStatus* WorldGeneratorStatus = nullptr;

	UFUNCTION()
	bool IsMissionActive();

public:
	// Used by the Ubiquity menu to see if the "Boot existing OS" screen should show.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Peacegate")
	static bool HasExistingOS();

	UFUNCTION(BlueprintCallable, Category = "Peacegate")
		static APeacenetWorldStateActor* GenerateAndCreateWorld(const APlayerController* InPlayerController, const FPeacenetWorldInfo& InWorldInfo, TSubclassOf<UDesktopWidget> InDesktop, UPeacenetGameTypeAsset* InGameType, TSubclassOf<UWindow> InWindowDecorator, FShowLoadingScreenEvent InShowLoadingScreen);

	UFUNCTION(BlueprintCallable, Category = "Peacegate")
	static APeacenetWorldStateActor* LoadExistingOS(const APlayerController* InPlayerController);
};

