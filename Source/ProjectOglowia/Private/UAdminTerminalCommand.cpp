#include "UAdminTerminalCommand.h"
#include "UUserContext.h"

void UAdminTerminalCommand::NativeRunCommand(UConsoleContext* InConsole, const TMap<FString, UDocoptValue*> InArguments)
{
	if (!InConsole->GetUserContext()->IsAdministrator())
	{
		InConsole->WriteLine(TEXT("error: must be run as root."));
		this->Complete();
		return;
	}

	Super::NativeRunCommand(InConsole, InArguments);
}