// copyright lololol

#include <PirateSurvivors.h>

#include "PirateLog.h"
#include "Modules/ModuleManager.h"

#if WITH_EDITOR
	#include "MessageLogModule.h"
#endif

DEFINE_LOG_CATEGORY(LogPirate);

IMPLEMENT_PRIMARY_GAME_MODULE( FPirateSurvivorsModule, PirateSurvivors, "Pirate Survivors" );

#define LOCTEXT_NAMESPACE "PirateSurvivors"

void FPirateSurvivorsModule::StartupModule()
{
#if WITH_EDITOR
	FMessageLogModule& MessageLogModule = FModuleManager::LoadModuleChecked<FMessageLogModule>("MessageLog");
	FMessageLogInitializationOptions InitOptions;
	InitOptions.bShowPages = true;
	InitOptions.bAllowClear = true;
	InitOptions.bShowFilters = true;
	MessageLogModule.RegisterLogListing("Pirate", NSLOCTEXT("PirateSurvivors", "PirateSurvivorsLogLabel", "Pirate Survivors - Main"), InitOptions);
#endif
	
	PIRATE_LOG("Started main Pirate Survivors module!");
}

void FPirateSurvivorsModule::ShutdownModule()
{
#if WITH_EDITOR
	if (FModuleManager::Get().IsModuleLoaded("MessageLog"))
	{
		// Unregister message log
		FMessageLogModule& MessageLogModule = FModuleManager::GetModuleChecked<FMessageLogModule>("MessageLog");
		MessageLogModule.UnregisterLogListing("Pirate");
	}
#endif
	
	PIRATE_LOG_WARN("Shutting down main Pirate Survivors module.");
}

#undef LOCTEXT_NAMESPACE