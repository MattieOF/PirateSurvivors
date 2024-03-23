// copyright lololol

#include <PirateSurvivors.h>

#include "PirateLog.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FPirateSurvivorsModule, PirateSurvivors, "Pirate Survivors" );

#define LOCTEXT_NAMESPACE "PirateSurvivors"

void FPirateSurvivorsModule::StartupModule()
{
	InitPirateMessageLog();
	
	PIRATE_LOG("Started main Pirate Survivors module!");
}

void FPirateSurvivorsModule::ShutdownModule()
{
	DeinitPirateMessageLog();
	
	PIRATE_LOG_WARN("Shutting down main Pirate Survivors module.");
}

#undef LOCTEXT_NAMESPACE
