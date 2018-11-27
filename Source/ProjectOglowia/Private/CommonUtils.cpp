// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "CommonUtils.h"
#include "Engine/Font.h"
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

