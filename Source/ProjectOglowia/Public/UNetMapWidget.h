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
#include "FPinnedContact.h"
#include "UNetMapWidget.generated.h"

class UDesktopWidget;

USTRUCT(BlueprintType)
struct PROJECTOGLOWIA_API FNetMapNode
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	float ColorValue = 0.f;

	UPROPERTY(BlueprintReadOnly)
	FPinnedContact Contact;
};

UENUM(BlueprintType)
enum class ENetMapDisplayType : uint8
{
	CountryMap,
	EnterpriseMap
};

UENUM(BlueprintType)
enum class ENetMapAttentionType : uint8
{
	Generic,
	NewMission
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FContactWantsAttentionEvent, FNetMapNode, InNode, bool, PlaySound, ENetMapAttentionType, AttentionType);

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

	UPROPERTY(BlueprintAssignable, Category = "NetMap")
	FContactWantsAttentionEvent ContactWantsAttention;

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "NetMap")
	void NodeAdded(const FNetMapNode InNode);

	UFUNCTION(BlueprintImplementableEvent, Category = "NetMap")
	void NodesCleared();

public:
	UFUNCTION(BlueprintCallable, Category = "NetMap")
	void SelectNode(const FNetMapNode& InNode);

	UFUNCTION()
	void CollectDiscoveredNodes();

	UFUNCTION()
	FVector2D CalculateLocation(const FPeacenetIdentity& InCharacter, const FComputer& InComputer);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "NetMap")
	bool GetCharacterData(const FNetMapNode& InNode, FPeacenetIdentity& OutCharacter);
	
	virtual void NativeConstruct() override;
};
