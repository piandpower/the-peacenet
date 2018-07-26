// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Engine/Font.h"

#include "Fonts/SlateFontInfo.h"
#include "SPeacegateTerminalWidget.h"
#include "Slate/SlateBrushAsset.h"
#include "Misc/Parse.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "PTerminalWidget.generated.h"

 //in editor
  
 //Underlying Slate terminal widget that does all the cool rendering shit.
 class SPeacegateTerminalWidget;
 
 /**
  * Terminal emulator widget for Peacegate OS.
  */
UCLASS()
class PROJECTOGLOWIA_API UPTerminalWidget : public UUserWidget
{
	GENERATED_BODY()
	
		
		public:
			UPTerminalWidget(const FObjectInitializer& ObjectInitializer);
		
			DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStdInCharWrittenEvent, const FText&, Character);
			DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStdInLineWrittenEvent, const FText&, Text);
			DECLARE_DYNAMIC_DELEGATE_RetVal(UFont*, FGetFont);
		
		public:
		 
		
			UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Fonts and Colors")
			UFont* RegularTextFont;
		
			UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fonts and Colors")
			UFont* BoldTextFont;
			
			UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fonts and Colors")
			UFont* ItalicTextFont;
			
			UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fonts and Colors")
			UFont* BoldItalicTextFont;
		
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
		
		
		public:
			/** Called when a character is written to the standard input stream. */
			UPROPERTY(BlueprintAssignable, Category="Widget Event", meta=(DisplayName = "On Standard Input Character Written"))
			FOnStdInCharWrittenEvent OnStdInCharWritten;
		
			/** Called when a line of text is written to the standard input stream. */
			UPROPERTY(BlueprintAssignable, Category = "Widget Event", meta = (DisplayName = "On Standard Input Line Written"))
			FOnStdInLineWrittenEvent OnStdInLineWritten;
		
			UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Console IO|Input")
			bool EchoInputText = true;
		
			UFUNCTION(BlueprintCallable, Category = "Console IO|Input",
				meta = (Latent, LatentInfo = "LatentInfo"))
			virtual void ReadLine(struct FLatentActionInfo LatentInfo, FString& OutText);
			 
			UFUNCTION(BlueprintCallable, Category="Console IO|Output")
			void Write(FString InText);
			
			UFUNCTION(BlueprintCallable, Category = "Console IO|Output")
			void WriteLine(FString InText);
			
			UFUNCTION(BlueprintCallable, Category = "Console IO|Output")
			void OverwriteLine(FString InText);
			
			UFUNCTION(BlueprintCallable, Category = "Console IO|Output")
			void Clear();
		
			UPROPERTY(BlueprintReadOnly)
			FVector2D GeometrySize;
			
			UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
			USlateBrushAsset * TerminalBrush;
		
			virtual void SynchronizeProperties() override;
			virtual void ReleaseSlateResources(bool bReleaseChildren) override;
			virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
			virtual void NativeConstruct() override;
			virtual void NativePaint(FPaintContext& Context) const override;
			virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
			virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
			virtual FReply NativeOnKeyChar(const FGeometry& InGeometry, const FCharacterEvent& InCharEvent) override;
			virtual bool NativeIsInteractable() const override { return true; }
			virtual bool NativeSupportsKeyboardFocus() const override { return true; }
		
		
		
		#if WITH_EDITOR
			virtual const FText GetPaletteCategory() override;
		#endif
		
			virtual TSharedRef<SWidget> RebuildWidget() override;
		
			UFUNCTION(BlueprintCallable)
			FString& GetInputText() { return TextInputBuffer; }
			void ClearInput() { TextInputBuffer.Empty(); }
		
		protected:
			TSharedPtr<SPeacegateTerminalWidget> MyPeacegateTerminalWidget;
		
			PROPERTY_BINDING_IMPLEMENTATION(FSlateFontInfo, RegularTextFont);
			PROPERTY_BINDING_IMPLEMENTATION(FSlateFontInfo, BoldTextFont);
			PROPERTY_BINDING_IMPLEMENTATION(FSlateFontInfo, ItalicTextFont);
			PROPERTY_BINDING_IMPLEMENTATION(FSlateFontInfo, BoldItalicTextFont);
		
		private:
			UFont* GetUnrealFont(uint8 fontType) const;
		
			FString TextBuffer;
			FString TextInputBuffer;
		
			//The width and height in pixels of a character.
			float CharacterWidth;
			float CharacterHeight;
		
			// The scroll offset used when rendering text.
			float ScrollOffsetY;
		
			// The maximum value that ScrollOffsetY can be. Minimum is always 0.
			float MaxScrollOffset;
		
			float GetLineHeight();
		
			bool ParseEscape(TCHAR character, uint8& termFont, uint8& termForegroundColorCode) const;
		
			TArray<FLinearColor> ColorPalette;	
	
	
};
