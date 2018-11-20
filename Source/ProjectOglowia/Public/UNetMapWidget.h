// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Rendering/DrawElements.h"
#include "FComputer.h"
#include "FPeacenetIdentity.h"
#include "ECountry.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Slate/SlateBrushAsset.h"
#include "Engine/Font.h"
#include "SlateFontInfo.h"
#include "UNetMapWidget.generated.h"

class UDesktopWidget;

UENUM(BlueprintType)
enum class ENetMapEmblemType : uint8
{
	Character,
	Business,
	Computer
};

USTRUCT(BlueprintType)
struct PROJECTOGLOWIA_API FNetMapNode
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	ENetMapEmblemType EmblemType = ENetMapEmblemType::Character;

	UPROPERTY(BlueprintReadOnly)
	FVector2D Location;

	UPROPERTY(BlueprintReadOnly)
	float ColorValue = 0.f;

	UPROPERTY(BlueprintReadOnly)
	int EntityID = 0;
};

UENUM(BlueprintType)
enum class ENetMapDisplayType : uint8
{
	CountryMap,
	EnterpriseMap
};

/**
 * A base widget for the Network Map.
 */
UCLASS(BlueprintType, Blueprintable, Abstract)
class PROJECTOGLOWIA_API UNetMapWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "NetMap")
	ENetMapDisplayType DisplayType = ENetMapDisplayType::CountryMap;

	UPROPERTY(BlueprintReadOnly, Category = "NetMap")
	UDesktopWidget* Desktop;

	UPROPERTY()
	TArray<FNetMapNode> Nodes;

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "NetMap")
	void NodeAdded(const FNetMapNode InNode);

	UFUNCTION(BlueprintImplementableEvent, Category = "NetMap")
	void NodesCleared();

public:
	UFUNCTION()
	void CollectDiscoveredNodes();

	UFUNCTION()
	FVector2D CalculateLocation(const FPeacenetIdentity& InCharacter, const FComputer& InComputer);

	virtual void NativeConstruct() override;
};
