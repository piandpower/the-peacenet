#include "URedirectedConsoleContext.h"
#include "UUserContext.h"
#include "CommonUtils.h"

void URedirectedConsoleContext::DumpToFile(UConsoleContext* InConsole)
{
	if (OutputFilePath.IsEmpty())
		return;

	this->OutputFilePath = this->CombineWithWorkingDirectory(this->OutputFilePath);

	UPeacegateFileSystem* DumpFS = this->GetUserContext()->GetFilesystem();

	EFilesystemStatusCode StatusCode = EFilesystemStatusCode::OK;
	if (Overwrite || !DumpFS->FileExists(OutputFilePath))
	{
		DumpFS->WriteText(OutputFilePath, this->GetLog());
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
		DumpFS->WriteText(OutputFilePath, OldText + this->GetLog());
	}

}
