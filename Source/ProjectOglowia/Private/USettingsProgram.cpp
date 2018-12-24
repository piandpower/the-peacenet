#include "USettingsProgram.h"
#include "USystemContext.h"
#include "UDesktopWidget.h"

void USettingsProgram::NativeProgramLaunched()
{
	this->Desktop = this->Window->SystemContext->Desktop;
}