// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Engine/Font.h"

#include "Fonts/SlateFontInfo.h"
#include "Slate/SlateBrushAsset.h"
#include "Misc/Parse.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "PTerminalWidget.generated.h"

 /**
  * Terminal emulator widget for Peacegate OS.
  */
UCLASS()
class PROJECTOGLOWIA_API UPTerminalWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UPTerminalWidget(const FObjectInitializer& ObjectInitializer);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExitEvent);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStdInCharWrittenEvent, const FText&, Character);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStdInLineWrittenEvent, const FText&, Text);
	DECLARE_DYNAMIC_DELEGATE_RetVal(UFont*, FGetFont);
	
private:
	UPROPERTY()
	float ZoomFactor = 1.f;

	UFUNCTION()
	FSlateFontInfo ZoomText(FSlateFontInfo InFont) const;

public:

	UFUNCTION(BlueprintCallable)
		void Exit();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fonts and Colors")
		FSlateFontInfo RegularTextFont;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fonts and Colors")
		FSlateFontInfo BoldTextFont;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fonts and Colors")
		FSlateFontInfo ItalicTextFont;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fonts and Colors")
		FSlateFontInfo BoldItalicTextFont;

	/** The time in milliseconds between each cursor blink. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
		float CursorBlinkTimeMS;

	UPROPERTY()
		FGetFont RegularTextFontDelegate;

	UPROPERTY()
		FGetFont BoldTextFontDelegate;

	UPROPERTY()
		FGetFont ItalicTextFontDelegate;

	UPROPERTY()
		FGetFont BoldItalicTextFontDelegate;

	//The width and height in pixels of a character.
	UPROPERTY(BlueprintReadOnly)
		float CharacterWidth;

	UPROPERTY(BlueprintReadOnly)
		float CharacterHeight;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int CommandLayer = 0;

	/** Called when a command or user requests to "exit" this Terminal. */
	UPROPERTY(BlueprintAssignable, Category = "Widget Event", meta = (DisplayName = "On Exit"))
		FOnExitEvent OnExit;


	/** Called when a character is written to the standard input stream. */
	UPROPERTY(BlueprintAssignable, Category = "Widget Event", meta = (DisplayName = "On Standard Input Character Written"))
		FOnStdInCharWrittenEvent OnStdInCharWritten;

	/** Called when a line of text is written to the standard input stream. */
	UPROPERTY(BlueprintAssignable, Category = "Widget Event", meta = (DisplayName = "On Standard Input Line Written"))
		FOnStdInLineWrittenEvent OnStdInLineWritten;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Console IO|Input")
		bool EchoInputText = true;

	UFUNCTION(BlueprintCallable, Category = "Console IO|Input",
		meta = (Latent, LatentInfo = "LatentInfo", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
		virtual void ReadLine(UObject* WorldContextObject, struct FLatentActionInfo LatentInfo, FString& OutText);


	UFUNCTION(BlueprintCallable, Category = "Console IO|Fancy Output",
		meta = (Latent, LatentInfo = "LatentInfo", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
		virtual UPTerminalWidget* SlowlyWriteText(UObject* WorldContextObject, struct FLatentActionInfo LatentInfo, const FString& InText, float InDelayTime = 0.05);

	UFUNCTION(BlueprintCallable, Category = "Console IO|Fancy Output",
		meta = (Latent, LatentInfo = "LatentInfo", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
		virtual UPTerminalWidget* SlowlyWriteLine(UObject* WorldContextObject, struct FLatentActionInfo LatentInfo, const FString& InText, float InDelayTime = 0.05);

	UFUNCTION(BlueprintCallable, Category = "Console IO|Fancy Output",
		meta = (Latent, LatentInfo = "LatentInfo", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
		virtual UPTerminalWidget* SlowlyOverwriteLine(UObject* WorldContextObject, struct FLatentActionInfo LatentInfo, const FString& InText, float InDelayTime = 0.05);


	UFUNCTION(BlueprintCallable, Category = "Console IO|Output")
		UPTerminalWidget* Write(FString InText);

	UFUNCTION(BlueprintCallable, Category = "Console IO|Output")
		UPTerminalWidget* WriteLine(FString InText);

	UFUNCTION(BlueprintCallable, Category = "Console IO|Output")
		UPTerminalWidget* OverwriteLine(FString InText);

	UFUNCTION(BlueprintCallable, Category = "Console IO|Output")
		UPTerminalWidget* Clear();

	UPROPERTY(BlueprintReadOnly)
		FVector2D GeometrySize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
		USlateBrushAsset * TerminalBrush;



	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	virtual void NativeConstruct() override;
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnKeyChar(const FGeometry& InGeometry, const FCharacterEvent& InCharEvent) override;
	virtual bool NativeIsInteractable() const override { return true; }
	virtual bool NativeSupportsKeyboardFocus() const override { return true; }



#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif
	UFUNCTION(BlueprintCallable)
		FString& GetInputText() { return TextInputBuffer; }
	void ClearInput() { TextInputBuffer = TEXT(""); }

protected:
	PROPERTY_BINDING_IMPLEMENTATION(FSlateFontInfo, RegularTextFont);
	PROPERTY_BINDING_IMPLEMENTATION(FSlateFontInfo, BoldTextFont);
	PROPERTY_BINDING_IMPLEMENTATION(FSlateFontInfo, ItalicTextFont);
	PROPERTY_BINDING_IMPLEMENTATION(FSlateFontInfo, BoldItalicTextFont);

private:
	bool bCursorActive = true;
	float cursorTime = 0;

	FSlateFontInfo GetUnrealFont(uint8 fontType) const;

	FString TextBuffer;
	FString TextInputBuffer;

	bool NewTextAdded = false;

	// The scroll offset used when rendering text.
	float ScrollOffsetY;

	// The maximum value that ScrollOffsetY can be. Minimum is always 0.
	float MaxScrollOffset;

	float GetLineHeight();
public:
	bool ParseEscape(TCHAR character, uint8& termFont, uint8& termForegroundColorCode) const;

private:
	TArray<FLinearColor> ColorPalette;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Common Utils|Terminal")
		static FString NewLine();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Common Utils|Terminal")
		static FString CarriageReturn();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Common Utils|Terminal")
		static FString Tab();

};
