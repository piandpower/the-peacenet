// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "CommonUtils.h"
#include "Engine/Font.h"
#include "UPeacenetSaveGame.h"
#include "Parse.h"
#include "USystemContext.h"

FText UCommonUtils::GetFriendlyFilesystemStatusCode(const EFilesystemStatusCode InStatusCode)
{
	switch (InStatusCode)
	{
	case EFilesystemStatusCode::OK:
		return FText();
	case EFilesystemStatusCode::DirectoryNotEmpty:
		return NSLOCTEXT("Peacegate", "DirectoryNotEmpty", "Directory not empty.");
	case EFilesystemStatusCode::FileOrDirectoryExists:
		return NSLOCTEXT("Peacegate", "FileOrDirectoryExists", "File or directory exists.");
	case EFilesystemStatusCode::FileOrDirectoryNotFound:
		return NSLOCTEXT("Peacegate", "FileOrDirectoryNotFound", "File or directory not found.");
	case EFilesystemStatusCode::PermissionDenied:
		return NSLOCTEXT("Peacegate", "PermissionDenied", "Permission denied.");


	default:
		return NSLOCTEXT("Peacegate", "UnknownError", "An unknown error has occurred.");
	}
}

UPeacegateFileSystem * UCommonUtils::CreateFilesystem(USystemContext* InSystemContext, int InUserID)
{
	UPeacegateFileSystem* FS = NewObject<UPeacegateFileSystem>();
	FS->SystemContext = InSystemContext;
	FS->Initialize(InUserID);
	return FS;
}

FLinearColor UCommonUtils::GetTerminalColor(ETerminalColor InColor)
{
	switch (InColor)
	{
	case ETerminalColor::Black:
	default:
		return FLinearColor::Black;
	case ETerminalColor::Blue:
		return FLinearColor(0.f, 0.f, 0.5f, 1.f);
	case ETerminalColor::Red:
		return FLinearColor(0.5f, 0.f, 0.f, 1.f);
	case ETerminalColor::Green:
		return FLinearColor(0.f, 0.5f, 0.f, 1.f);
	case ETerminalColor::Aqua:
		return FLinearColor(0.f, 0.5f, 0.5f, 1.f);
	case ETerminalColor::Purple:
		return FLinearColor(0.5f, 0.f, 0.5f, 1.f);
	case ETerminalColor::Yellow:
		return FLinearColor(0.5f, 0.5f, 0.f, 1.f);
	case ETerminalColor::Gray:
		return FLinearColor(0.5f, 0.5f, 0.5f, 1.f);
	case ETerminalColor::White:
		return FLinearColor(0.75F, 0.75f, 0.75f, 1.f);
	case ETerminalColor::LightBlue:
		return FLinearColor(0.f, 0.f, 1.f, 1.f);
	case ETerminalColor::LightGreen:
		return FLinearColor(0.f, 1.f, 0.f, 1.f);
	case ETerminalColor::LightRed:
		return FLinearColor(1.f, 0.f, 0.f, 1.f);
	case ETerminalColor::LightAqua:
		return FLinearColor(0.f, 1.f, 1.f, 1.f);
	case ETerminalColor::LightPurple:
		return FLinearColor(1.f, 0.f, 1.f, 1.f);
	case ETerminalColor::LightYellow:
		return FLinearColor(1.f, 1.f, 0.f, 1.f);
	case ETerminalColor::BrightWhite:
		return FLinearColor(1.f, 1.f, 1.f, 1.f);
	}
}

FString UCommonUtils::GetTerminalColorCode(ETerminalColor InColor)
{
	switch (InColor)
	{
	case ETerminalColor::Black:
	default:
		return "&0";
	case ETerminalColor::Blue:
		return "&1";
	case ETerminalColor::Red:
		return "&4";
	case ETerminalColor::Green:
		return "&2";
	case ETerminalColor::Aqua:
		return "&3";
	case ETerminalColor::Purple:
		return "&5";
	case ETerminalColor::Yellow:
		return "&6";
	case ETerminalColor::Gray:
		return "&8";
	case ETerminalColor::White:
		return "&7";
	case ETerminalColor::LightBlue:
		return "&9";
	case ETerminalColor::LightGreen:
		return "&A";
	case ETerminalColor::LightRed:
		return "&C";
	case ETerminalColor::LightAqua:
		return "&B";
	case ETerminalColor::LightPurple:
		return "&D";
	case ETerminalColor::LightYellow:
		return "&E";
	case ETerminalColor::BrightWhite:
		return "&F";
	}
}

bool UCommonUtils::IsColorCode(FString InControlCode, ETerminalColor& OutColor)
{
	if (!InControlCode.StartsWith("&"))
		return false;

	if (InControlCode.Len() != 2)
		return false;

	// Get rid of the "&" at the start so we can parse as hex
	InControlCode.RemoveAt(0, 1);

	int Code = FParse::HexNumber(InControlCode.GetCharArray().GetData());
	if (Code == 0 && InControlCode != "0")
		return false; //HexNumber returns 0 if the code is invalid.

	// Enums are awesome because we can do this.
	OutColor = (ETerminalColor)Code;
	return true;
}

void UCommonUtils::MeasureChar(const TCHAR InChar, const FSlateFontInfo & InSlateFont, float & OutWidth, float & OutHeight)
{
	float x, y = 0;

	const UFont* RawFont = Cast<UFont>(InSlateFont.FontObject);

	float MeasureSize = RawFont->LegacyFontSize;
	float RealSize = InSlateFont.Size;
	float Scale = RealSize / MeasureSize;

	RawFont->GetCharSize(InChar, x, y);

	OutWidth = x * Scale;
	OutHeight = y * Scale;
}

void UCommonUtils::SendEmailChecked(UPeacenetSaveGame * InSaveGame, int FromEntity, int ToEntity, const FText & Subject, const FText & Message, TArray<FEmailAttachment> InAttachments, TArray<FEmailMission> InMissions)
{
	FPeacenetIdentity FromCharacter;
	FPeacenetIdentity ToCharacter;

	// Do these character entities exist?
	check(InSaveGame->GetCharacterByID(FromEntity, FromCharacter));
	check(InSaveGame->GetCharacterByID(ToEntity, ToCharacter));

	FEmailMessage NewMessage;
	NewMessage.EntityID = InSaveGame->Emails.Num();
	NewMessage.Subject = Subject;
	NewMessage.Message = Message;
	NewMessage.Attachments = InAttachments;
	NewMessage.Missions = InMissions;
	NewMessage.IsUnread = true;
	
	InSaveGame->Emails.Add(NewMessage);
}

