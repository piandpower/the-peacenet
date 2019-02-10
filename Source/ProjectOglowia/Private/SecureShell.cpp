#include "SecureShell.h"

void USecureShell::StartSecureShell(ATerminalCommand* InCaller, FName InShellCommand, FAuthenticationRequiredEvent InCallback)
{
    check(InCaller);
    check(!this->OwningCommand);

    this->OwningCommand = InCaller;
    this->Shell = InShellCommand;

    this->StartAuth(InCallback);
}