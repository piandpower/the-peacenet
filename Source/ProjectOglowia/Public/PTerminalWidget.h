/********************************************************************************
 * The Peacenet - bit::phoenix("software");
 * 
 * MIT License
 *
 * Copyright (c) 2018-2019 Michael VanOverbeek, Declan Hoare, Ian Clary, 
 * Trey Smith, Richard Moch, Victor Tran and Warren Harris
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * Contributors:
 *  - Michael VanOverbeek <alkaline@bitphoenixsoftware.com>
 *
 ********************************************************************************/


#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ETerminalColor.h"
#include "Engine/Font.h"
#include "ThreadSafeBool.h"
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

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExitEvent);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTerminalZoomedEvent, float, InNewCharWidth, float, InNewCharHeight);

	
public: //variables
	FThreadSafeBool IsInputLineAvailable = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Appearance")
		bool bRenderBackground = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fonts and Colors")
		FSlateFontInfo RegularTextFont;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fonts and Colors")
		FSlateFontInfo BoldTextFont;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fonts and Colors")
		FSlateFontInfo ItalicTextFont;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fonts and Colors")
		FSlateFontInfo BoldItalicTextFont;

	UFUNCTION()
	int GetCharIndexAtPosition(const FGeometry InGeometry, FVector2D InPosition);

	UPROPERTY()
	int SelectionStart = -1;

	UPROPERTY()
	int SelectionEnd = -1;

	/** The time in milliseconds between each cursor blink. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
		float CursorBlinkTimeMS;

	//The width and height in pixels of a character.
	UPROPERTY(BlueprintReadOnly)
		float CharacterWidth;

	UPROPERTY(BlueprintReadOnly)
		float CharacterHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
	USlateBrushAsset * TerminalBrush;

	/** Called when a command or user requests to "exit" this Terminal. */
	UPROPERTY(BlueprintAssignable, Category = "Widget Event", meta = (DisplayName = "On Exit"))
		FOnExitEvent OnExit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Console IO|Input")
		bool EchoInputText = true;

	UPROPERTY(BlueprintAssignable)
		FTerminalZoomedEvent TerminalZoomedEvent;

private: //variables
	float ZoomFactor = 1.f;
	FVector2D GeometrySize;
	bool bCursorActive = true;
	float cursorTime = 0;
	FString TextBuffer;
	FString TextInputBuffer;
	bool Selecting = false;
	bool NewTextAdded = false;
	float ScrollOffsetY;
	float MaxScrollOffset;

public:

private: //functions
	bool SkipControlCode(const FString& InBuffer, int& InIndex, bool& OutLiteral) const;
	bool ParseControlCode(const FString& InBuffer, int& InIndex, ETerminalColor& OutColor, FSlateFontInfo& OutFont, bool& OutInvert, bool& OutAttention, bool& OutLiteral) const;
	float GetLineHeight();

	UFUNCTION()
	FSlateFontInfo ZoomText(FSlateFontInfo InFont) const;


public: //functions
	void InjectInput(const FString& Input);
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	virtual void NativeConstruct() override;
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnKeyChar(const FGeometry& InGeometry, const FCharacterEvent& InCharEvent) override;
	virtual bool NativeIsInteractable() const override { return true; }
	virtual bool NativeSupportsKeyboardFocus() const override { return true; }
	virtual FReply NativeOnMouseButtonUp( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	virtual FReply NativeOnMouseMove( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	virtual FReply NativeOnMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	void ClearInput() { TextInputBuffer = TEXT(""); }

	FString Sanitize(FString InDirtyBitch);

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

	UFUNCTION(BlueprintCallable)
		void Exit();

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

	UFUNCTION(BlueprintCallable)
		FString& GetInputText() { return TextInputBuffer; }

	UFUNCTION(BlueprintImplementableEvent, Category = "Terminal")
	void TerminalZoomed(float NewCharWidth, float NewCharHeight);

public: //static functions
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Common Utils|Terminal")
		static FString NewLine();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Common Utils|Terminal")
		static FString CarriageReturn();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Common Utils|Terminal")
		static FString Tab();

};
