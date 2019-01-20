// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UDesktopWidget.h"
#include "FManPage.h"
#include "UPeacenetSaveGame.h"
#include "UGameTypeAsset.h"
#include "PeacenetWorldStateActor.generated.h"

class USystemContext;
class UWallpaperAsset;
class UPeacegateProgramAsset;
class UTerminalCommand;
class UCommandInfo;
class UWindow;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerSystemContextReadyEvent, USystemContext*, InSystemContext);

UCLASS()
class PROJECTOGLOWIA_API APeacenetWorldStateActor : public AActor
{
	GENERATED_BODY()
	
private:
	UFUNCTION()
	void LoadTerminalCommands();

public:	
	UFUNCTION()
	void SaveWorld();

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

	// Sets default values for this actor's properties
	APeacenetWorldStateActor();

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
	UFUNCTION()
	bool FindProgramByName(FName InName, UPeacegateProgramAsset*& OutProgram);

	FText GetTimeOfDay();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Peacenet")
	void StartGame(TSubclassOf<UDesktopWidget> InDesktopClass, TSubclassOf<UWindow> InWindowClass);
	
public:
	// Used by the Ubiquity menu to see if the "Boot existing OS" screen should show.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Peacegate")
	static bool HasExistingOS();

	UFUNCTION(BlueprintCallable, Category = "Peacegate")
	static APeacenetWorldStateActor* LoadExistingOS(const APlayerController* InPlayerController);
};

