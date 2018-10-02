// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "SlateBrush.h"
#include "SlateFontInfo.h"
#include "UPeacegateThemeAsset.generated.h"
/**
 * Encapsulates a set of textures, colors, fonts and other data for a Peacegate OS system theme.
 */
UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UPeacegateThemeAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Metadata")
	FName ThemeID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Metadata")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Metadata")
	bool UnlockedByDefault;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fonts")
	FSlateFontInfo RegularSystemFont;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fonts")
	FSlateFontInfo FirstLevelHeadingFont;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fonts")
	FSlateFontInfo SecondLevelHeadingFont;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fonts")
	FSlateFontInfo ThirdLevelHeadingFont;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fonts")
	FSlateFontInfo TerminalRegularFont;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fonts")
	FSlateFontInfo TerminalBoldFont;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fonts")
	FSlateFontInfo TerminalItalicFont;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fonts")
	FSlateFontInfo TerminalBoldItalicFont;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fonts")
	FSlateFontInfo WindowTitleFont;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fonts")
	FSlateFontInfo SmallFont;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Panel")
	FSlateBrush PanelBackground;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Metadata")
	FLinearColor PanelForegroundColor;


};
