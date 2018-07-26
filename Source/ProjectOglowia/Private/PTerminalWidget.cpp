// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "PTerminalWidget.h"
#include "FConsoleReadLineLatentAction.h"


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

void UPTerminalWidget::ReadLine(struct FLatentActionInfo LatentInfo, FString& OutText)
{
	//You might have an issue here, with getting the world from a UObject, if I remember correctly
	//The Uobject needs to implement a GetWorld(); function
	//UObject, has a blank GetWorld(); function by default
	//If you know exactly what the object is, you would be better off using that instead of the 
	//Generic UObject class! (Because it likely already has a GetWorld() setup.....
	//UWorld* world = GEngine->GetWorldFromContextObject(WorldContextObject);
	UWorld* world = this->GetWorld();
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

void UPTerminalWidget::Write(FString InText)
{
	TextBuffer.Append(InText);
}

void UPTerminalWidget::WriteLine(FString InText)
{
	Write(InText + TEXT("\n"));
}

void UPTerminalWidget::OverwriteLine(FString InText)
{
	Write(InText + TEXT("\r"));
}

void UPTerminalWidget::Clear()
{
	TextBuffer.Empty(0);
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
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, TEXT("Terminal Emulator: Received focus."));
	return FReply::Handled();
}

void UPTerminalWidget::NativeConstruct()
{
	SetVisibility(ESlateVisibility::Visible);

	APlayerController* ctrl = GetOwningPlayer();

	ctrl->SetInputMode(FInputModeUIOnly());

	Super::NativeConstruct();
}

void UPTerminalWidget::NativePaint(FPaintContext& context) const
{
	// Get the alotted geometry for the paint context.
	FGeometry geometry = context.AllottedGeometry;

	// Get the size in pixels of the geometry so we know how big our widget is.
	FVector2D size = geometry.GetLocalSize();

	uint8 term_font = 0; // What kind of font are we using?
	uint8 term_bg_color = 0; //What entry in the color palette are we using for the background?
	uint8 term_fg_color = 1; //What entry in the color palette are we using for the foreground?

	float char_x = 0.f; //Where will the character be rendered on the X coordinate?
	float char_y = 0.f - ScrollOffsetY; //The current text position on the Y axis, accounting for the scroll offset.

	bool bSkipEscape = false; //Should we skip reading an escape sequence when we come across a char '`'?

	//This fills the entire widget with a black background.
	UWidgetBlueprintLibrary::DrawBox(context, FVector2D(0.f, 0.f), size, TerminalBrush, FLinearColor(0.f, 0.f, 0.f, 1.f));

	UFont* font = this->GetUnrealFont(term_font); //Grab the regular font.

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
		if (c == TEXT('\t') || c == TEXT('\v'))
			continue;

		//If char_x + char_w is greater than our width, drop down to a new line.
		if (char_x + char_w > size.X)
		{
			char_x = 0;
			char_y += char_h;
		}

		//update font from font code
		font = this->GetUnrealFont(term_font);

		//Render it.
		UWidgetBlueprintLibrary::DrawTextFormatted(context, FText::FromString(FString::Chr(c)), FVector2D(char_x, char_y), font, 22, ((FName)(L"Regular")), ColorPalette[term_fg_color]);

		//Advance the x coordinate by a single char.
		char_x += char_w;
	}

	//One last time so the cursor isn't cut off and is an accurate representation of where new text will be placed.
	if (char_x + char_w > size.X)
	{
		char_x = 0;
		char_y += char_h;
	}

	UWidgetBlueprintLibrary::DrawBox(context, FVector2D(char_x, char_y), FVector2D(char_w, char_h), TerminalBrush, ColorPalette[term_fg_color]);
}

void UPTerminalWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//Grab the width and height in pixels of the char '#'. Ideally, all terminals use monospace fonts, so, every character should just be the same size.
	//I could program this to grab the size of every character as I render them but I don't want to do that.
	//Plus this will kinda make things faster.
	//
	//Just use a fucking monospace font, god.
	RegularTextFont->GetCharSize(TEXT('#'), CharacterWidth, CharacterHeight);

	FVector2D gSize = MyGeometry.GetLocalSize();

	if (GeometrySize != gSize)
	{
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
			}
		}
	}
	else if (c == TEXT('\r'))
	{
		TextInputBuffer = TextInputBuffer.AppendChar(TEXT('\n'));
		TextBuffer = TextBuffer.AppendChar(TEXT('\n'));
	}
	else {
		TextInputBuffer = TextInputBuffer.AppendChar(c);
		if (EchoInputText)
		{
			TextBuffer = TextBuffer.AppendChar(c);
		}
	}

	return FReply::Handled();
}

#if WITH_EDITOR
const FText UPTerminalWidget::GetPaletteCategory()
{
	return FText::FromString(TEXT("Peacegate OS"));
}
TSharedRef<SWidget> UPTerminalWidget::RebuildWidget()
{
	return Super::RebuildWidget();
}

UFont* UPTerminalWidget::GetUnrealFont(uint8 fontType) const
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

	return nullptr;
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
#endif

