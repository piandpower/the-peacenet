#include "URedirectedConsoleContext.h"

void URedirectedConsoleContext::DumpToFile(UConsoleContext* InConsole)
{
	if (OutputFilePath.IsEmpty())
		return;

	this->OutputFilePath = this->CombineWithWorkingDirectory(this->OutputFilePath);

	UPeacegateFileSystem* DumpFS = this->Filesystem;

	EFilesystemStatusCode StatusCode = EFilesystemStatusCode::OK;
	if (Overwrite || !DumpFS->FileExists(OutputFilePath))
	{
		DumpFS->WriteText(OutputFilePath, Log);
	}
	else
	{
		FString OldText;
		DumpFS->ReadText(OutputFilePath, OldText, StatusCode);
		if (StatusCode != EFilesystemStatusCode::OK)
		{
			InConsole->WriteLine("`3`*error: " + OutputFilePath + ": " + UCommonUtils::GetFriendlyFilesystemStatusCode(StatusCode).ToString() + "`1`r");
			return;
		}
		DumpFS->WriteText(OutputFilePath, OldText + Log);
	}

}
