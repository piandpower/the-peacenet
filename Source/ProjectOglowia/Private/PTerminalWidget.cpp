// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "PTerminalWidget.h"
#include "FConsoleReadLineLatentAction.h"
#include "Rendering/DrawElements.h"
#include "FTerminalSlowTypeLatentAction.h"


UPTerminalWidget::UPTerminalWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{ 
	//Initialize color palette
	ColorPalette = TArray<FLinearColor>();
	ColorPalette.Add(FLinearColor::Black);
	ColorPalette.Add(FLinearColor::White);
	ColorPalette.Add(FLinearColor::Gray);
	ColorPalette.Add(FLinearColor::Red);
	ColorPalette.Add(FLinearColor::Green);
	ColorPalette.Add(FLinearColor::Blue);
	ColorPalette.Add(FLinearColor(FColor(0xFFFFA500)));
	ColorPalette.Add(FLinearColor::Yellow);
	ColorPalette.Add(FLinearColor(FColor(0xFFADD8E6)));
	ColorPalette.Add(FLinearColor(FColor(0xFF98FB98)));
	ColorPalette.Add(FLinearColor(FColor(0xFFFF69B4)));
	ColorPalette.Add(FLinearColor(FColor(0xFFA020F0)));
	ColorPalette.Add(FLinearColor(FColor(0xFFFFB6C1)));
	ColorPalette.Add(FLinearColor(FColor(0xFFCF0000)));
	ColorPalette.Add(FLinearColor(FColor(0xFF0000CF)));
	ColorPalette.Add(FLinearColor(FColor(0xFF00CF00)));


}

void UPTerminalWidget::Exit()
{
	OnExit.Broadcast();
}

void UPTerminalWidget::ReadLine(UObject* WorldContextObject, struct FLatentActionInfo LatentInfo, FString& OutText)
{
	if (WorldContextObject) 
	{
		UWorld* world = WorldContextObject->GetWorld();
		if (world)
		{
			FLatentActionManager& LatentActionManager = world->GetLatentActionManager();
			if (LatentActionManager.FindExistingAction<FConsoleReadLineLatentAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) == NULL)
			{

				//Here in a second, once I confirm the project loads, we need to see whats wrong with this
				LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, new FConsoleReadLineLatentAction(this, LatentInfo, OutText));
			}
		}
	}
}

UPTerminalWidget * UPTerminalWidget::SlowlyWriteText(UObject * WorldContextObject, FLatentActionInfo LatentInfo, const FString & InText, float InDelayTime)
{
	if (WorldContextObject)
	{
		UWorld* world = WorldContextObject->GetWorld();
		if (world)
		{
			FLatentActionManager& LatentActionManager = world->GetLatentActionManager();
			if (LatentActionManager.FindExistingAction<FTerminalSlowTypeLatentAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) == NULL)
			{

				//Here in a second, once I confirm the project loads, we need to see whats wrong with this
				LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, new FTerminalSlowTypeLatentAction(this, LatentInfo, InText, InDelayTime));
			}
		}
	}
	return this;

}

UPTerminalWidget * UPTerminalWidget::SlowlyWriteLine(UObject * WorldContextObject, FLatentActionInfo LatentInfo, const FString & InText, float InDelayTime)
{
	return SlowlyWriteText(WorldContextObject, LatentInfo, InText + TEXT("\n"), InDelayTime);
}

UPTerminalWidget * UPTerminalWidget::SlowlyOverwriteLine(UObject * WorldContextObject, FLatentActionInfo LatentInfo, const FString & InText, float InDelayTime)
{
	return SlowlyWriteText(WorldContextObject, LatentInfo, InText + TEXT("\r"), InDelayTime);
}

UPTerminalWidget* UPTerminalWidget::Write(FString InText)
{
	TextBuffer.Append(InText);
	NewTextAdded = true;

	bCursorActive = true;
	cursorTime = 0;

	return this;
}

UPTerminalWidget* UPTerminalWidget::WriteLine(FString InText)
{
	return Write(InText + TEXT("\n"));
}

UPTerminalWidget* UPTerminalWidget::OverwriteLine(FString InText)
{
	return Write(InText + TEXT("\r"));
}

UPTerminalWidget* UPTerminalWidget::Clear()
{
	TextBuffer.Empty(0);
	NewTextAdded = true;
	return this;
}

void UPTerminalWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();
}

void UPTerminalWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyPeacegateTerminalWidget.Reset();
}

FReply UPTerminalWidget::NativeOnFocusReceived(const FGeometry & InGeometry, const FFocusEvent & InFocusEvent)
{
	cursorTime = 0;
	bCursorActive = true;

	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, TEXT("Terminal Emulator: Received focus."));
	return FReply::Handled();
}

void UPTerminalWidget::NativeConstruct()
{
	const UFont* RegularUnrealFont = Cast<UFont>(RegularTextFont.FontObject);
	
	if(RegularUnrealFont)
	{
		//This is where we set the initial character size if we have a regular text font.
		//If we don't have a regular text font at this point then fuck the frontend devs.
		RegularUnrealFont->GetCharSize(TEXT('#'), CharacterWidth, CharacterHeight);

	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, TEXT("WARNING: Terminal Emulator::Initialize(): Regular font not set, couldn't measure default character size. (Did you forget to set your fonts in the UMG designer?)"));
	}

	SetVisibility(ESlateVisibility::Visible);

	Super::NativeConstruct();
}

int32 UPTerminalWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	// Get the size in pixels of the geometry so we know how big our widget is.
	FVector2D size = AllottedGeometry.GetLocalSize();

	uint8 term_font = 0; // What kind of font are we using?
	uint8 term_bg_color = 0; //What entry in the color palette are we using for the background?
	uint8 term_fg_color = 1; //What entry in the color palette are we using for the foreground?

	float char_x = 0.f; //Where will the character be rendered on the X coordinate?
	float char_y = 0.f - ScrollOffsetY; //The current text position on the Y axis, accounting for the scroll offset.

	bool bSkipEscape = false; //Should we skip reading an escape sequence when we come across a char '`'?

	//Draws the background of the terminal.
	LayerId++;

	if (TerminalBrush)
	{
		FSlateDrawElement::MakeBox(
			OutDrawElements,
			LayerId,
			AllottedGeometry.ToPaintGeometry(FVector2D::ZeroVector, size),
			&TerminalBrush->Brush,
			ESlateDrawEffect::None,
			FLinearColor::Black);
	}


	FSlateFontInfo font = this->GetUnrealFont(term_font); //Grab the regular font.

	//The width of a character.
	float char_w = CharacterWidth;
	//The height of a character.
	float char_h = CharacterHeight;

	TArray<TCHAR> arr = TextBuffer.GetCharArray();

	//How many elements are in the text array?
	int arrNum = arr.Num();

	for (int i = 0; i < arrNum; i++)
	{

		//Get the character.
		TCHAR c = arr[i];

		// Handle reading an escape sequence if the current char is a backtick (`).
		if (c == TEXT('`')) {
			if (bSkipEscape)
			{
				//Skip this escape, but set bSkipEscape to false for the next value.
				bSkipEscape = false;
			}
			else {
				//If we're not the last char in the array:
				if (i < arrNum - 1)
				{
					//Seek ahead to the next char.
					TCHAR nextChar = arr[i + 1];

					//Skip if it's a backtick. Double-backticks mean "render a literal backtick."
					if (nextChar == TEXT('`'))
					{
						bSkipEscape = true;
						continue;
					}

					//Attempt to parse the escape sequence. If it succeeds, i += 1 and continue.
					if (ParseEscape(nextChar, term_font, term_fg_color))
					{
						i++;
						continue;
					}
				}
			}
		}

		// If the character is a tab ('\t'), handle it.
		if (c == TEXT('\t'))
		{
			// Get remainder of char_x divided by 8 * char_w.
			float space = FMath::Fmod(char_x, char_w * 8);

			// Realistically, truncating the value won't make much of a difference.
			// But we'll store it as a float simply to make Unreal's life easier.
			// All we need to do is add this value to char_x.
			char_x += (char_w * 8) - space;

			// Go to next char.
			continue;
		}

		//return to char 0 if we're a \r
		if (c == TEXT('\r'))
		{
			char_x = 0;
			continue;
		}

		if (c == TEXT('\0'))
			break; //Stop rendering at a null terminator because this is C++. I hate this.

		//Drop down to a new line.
		if (c == TEXT('\n'))
		{
			char_x = 0;
			char_y += char_h;
			continue;
		}

		//Skip vertical tabs.
		if (c == TEXT('\v'))
			continue;

		//If char_x + char_w is greater than our width, drop down to a new line.
		if (char_x + char_w > size.X)
		{
			char_x = 0;
			char_y += char_h;
		}

		//If char_y is < 0, there is no reason to do any of this except move char_x by char_w.
		if (char_y < 0.f)
		{
			char_x += char_w;
			continue;
		}

		//if char_y is >= our control's height, we have NO REASON to continue rendering.
		if (char_y >= size.Y)
			break;

		//update font from font code
		font = this->GetUnrealFont(term_font);

		LayerId++;

		FSlateDrawElement::MakeText(
			OutDrawElements,
			LayerId,
			AllottedGeometry.ToOffsetPaintGeometry(FVector2D(char_x, char_y)),
			FText::FromString(FString::Chr(c)),
			font,
			ESlateDrawEffect::None,
			ColorPalette[term_fg_color]);



		//Advance the x coordinate by a single char.
		char_x += char_w;
	}

	//One last time so the cursor isn't cut off and is an accurate representation of where new text will be placed.
	if (char_x + char_w > size.X)
	{
		char_x = 0;
		char_y += char_h;
	}


	if (TerminalBrush && bCursorActive)
	{
		LayerId++;

		FSlateDrawElement::MakeBox(
			OutDrawElements,
			LayerId,
			AllottedGeometry.ToPaintGeometry(FVector2D(char_x, char_y), FVector2D(char_w, char_h)),
			&TerminalBrush->Brush,
			ESlateDrawEffect::None,
			ColorPalette[term_fg_color]);

		if (!HasAnyUserFocus())
		{
			LayerId++;

			FSlateDrawElement::MakeBox(
				OutDrawElements,
				LayerId,
				AllottedGeometry.ToPaintGeometry(FVector2D(char_x+2, char_y+2), FVector2D(char_w-4, char_h-4)),
				&TerminalBrush->Brush,
				ESlateDrawEffect::None,
				ColorPalette[term_bg_color]);

		}
	}


	return LayerId;
}

void UPTerminalWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (HasAnyUserFocus())
	{
		cursorTime += InDeltaTime;
		if (cursorTime >= CursorBlinkTimeMS)
		{
			cursorTime = 0;
			bCursorActive = !bCursorActive;
		}
	}
	else {
		cursorTime = 0;
		bCursorActive = true;
	}

	//Grab the width and height in pixels of the char '#'. Ideally, all terminals use monospace fonts, so, every character should just be the same size.
	//I could program this to grab the size of every character as I render them but I don't want to do that.
	//Plus this will kinda make things faster.
	//
	//Just use a fucking monospace font, god.
	const UFont* RegularUnrealFont = Cast<UFont>(RegularTextFont.FontObject);

	if (RegularUnrealFont)
	{
		//This is where we set the initial character size if we have a regular text font.
		//If we don't have a regular text font at this point then fuck the frontend devs.
		RegularUnrealFont->GetCharSize(TEXT('#'), CharacterWidth, CharacterHeight);

	}

	FVector2D gSize = MyGeometry.GetLocalSize();

	if (GeometrySize != gSize || NewTextAdded)
	{
		NewTextAdded = false;
		GeometrySize = gSize;

		float h = GetLineHeight();

		MaxScrollOffset = FMath::Max(0.f, h - GeometrySize.Y);

		ScrollOffsetY = MaxScrollOffset;
	}



}

FReply UPTerminalWidget::NativeOnMouseWheel(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	float wheelDelta = InMouseEvent.GetWheelDelta();

	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("Terminal emulator: Mouse scrolled. Delta: "));
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::SanitizeFloat(wheelDelta));


	ScrollOffsetY = FMath::Clamp(ScrollOffsetY - (wheelDelta*CharacterHeight), 0.f, MaxScrollOffset);

	return FReply::Handled();
}

FReply UPTerminalWidget::NativeOnKeyChar(const FGeometry & InGeometry, const FCharacterEvent & InCharEvent)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Terminal key char event: ") + FString::Chr(InCharEvent.GetCharacter()));
	
	TCHAR c = InCharEvent.GetCharacter();

	if (c == TEXT('\b'))
	{
		if (!TextInputBuffer.IsEmpty())
		{
			TextInputBuffer.RemoveAt(TextInputBuffer.Len() - 1, 1, true);
			if (EchoInputText)
			{
				TextBuffer.RemoveAt(TextBuffer.Len() - 1, 1, true);
				NewTextAdded = true;
			}
		}
	}
	else if (c == TEXT('\r'))
	{
		TextInputBuffer = TextInputBuffer.AppendChar(TEXT('\n'));
		TextBuffer = TextBuffer.AppendChar(TEXT('\n'));
		NewTextAdded = true;
	}
	else {
		TextInputBuffer = TextInputBuffer.AppendChar(c);
		if (EchoInputText)
		{
			TextBuffer = TextBuffer.AppendChar(c);
			NewTextAdded = true;
		}
	}

	bCursorActive = true;
	cursorTime = 0;

	return FReply::Handled();
}

#if WITH_EDITOR
const FText UPTerminalWidget::GetPaletteCategory()
{
	return FText::FromString(TEXT("Peacegate OS"));
}
#endif

TSharedRef<SWidget> UPTerminalWidget::RebuildWidget()
{
	return Super::RebuildWidget();
}

FSlateFontInfo UPTerminalWidget::GetUnrealFont(uint8 fontType) const
{
	switch (fontType)
	{
	case 0:
		return RegularTextFont;
	case 1:
		return BoldTextFont;
	case 2:
		return ItalicTextFont;
	case 3:
		return BoldItalicTextFont;
	}

	return RegularTextFont;
}

float UPTerminalWidget::GetLineHeight()
{
	float maxWidth = GeometrySize.X;

	float x = 0;
	float y = 0;

	float char_w = CharacterWidth;
	float char_h = CharacterHeight;

	bool skipEscape = false;

	TArray<TCHAR> arr = TextBuffer.GetCharArray();

	int arrNum = arr.Num();

	uint8 dummy = 0;

	for (int i = 0; i < arrNum; i++)
	{
		TCHAR c = arr[i];

		if (c == TEXT('`'))
		{
			if (skipEscape)
			{
				skipEscape = false;
			}
			else {
				if (i < arrNum - 1)
				{
					TCHAR e = arr[i + 1];

					if (e == TEXT('`'))
					{
						skipEscape = true;
						continue;
					}

					if (ParseEscape(e, dummy, dummy))
					{
						i++;
						continue;
					}
				}
			}
		}

		if (c == TEXT('\r'))
		{
			x = 0;
			continue;
		}

		if (c == TEXT('\n'))
		{
			x = 0;
			y += char_h;
		}

		//if tab, backspace, null or vertical tab, fuck off.
		if (c == TEXT('\t') || c == TEXT('\v') || c == TEXT('\b') || c == TEXT('\0'))
			continue;

		if (x + char_w > maxWidth)
		{
			x = 0;
			y += char_h;
		}
		else {
			x += char_w;
		}

	}

	return y + char_h;
}

bool UPTerminalWidget::ParseEscape(TCHAR character, uint8 & termFont, uint8 & termForegroundColorCode) const
{
	switch (character)
	{
	case TEXT('*'):
		termFont = 1;
		return true;
	case TEXT('-'):
		termFont = 3;
		return true;
	case TEXT('_'):
		termFont = 2;
		return true;
	case TEXT('r'):
		termFont = 0;
		return true;
	default:
		int32 hex = FParse::HexDigit(character);
		if (hex == 0)
		{
			//Special case: FParse returns 0 if it fails at parsing.
			//So if we get 0 we need to check our input char and make sure it was 0.
			//If it is then the foreground color becomes black. If not then we return false.
			if (character == TEXT('0'))
			{
				termForegroundColorCode = 0;
				return true;
			}
			else {
				return false;
			}
		}
		//If we get this far then we're a valid color code.
		termForegroundColorCode = (uint8)hex;
		return true;
	}
}

FString UPTerminalWidget::NewLine()
{
	return TEXT("\n");
}

FString UPTerminalWidget::CarriageReturn()
{
	return TEXT("\r");
}

FString UPTerminalWidget::Tab()
{
	return TEXT("\t");
}

