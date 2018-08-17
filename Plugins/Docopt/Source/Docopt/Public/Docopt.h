#pragma once

#include "DocoptPrivatePCH.h"
#include "ModuleManager.h"

class DocoptImpl : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	void StartupModule();
	void ShutdownModule();
};
