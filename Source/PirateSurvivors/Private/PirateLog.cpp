#include "PirateLog.h"

#if WITH_EDITOR
	#include "MessageLogModule.h"
#endif

DEFINE_LOG_CATEGORY(LogPirate);

void InitPirateMessageLog()
{
#if WITH_EDITOR
	FMessageLogModule& MessageLogModule = FModuleManager::LoadModuleChecked<FMessageLogModule>("MessageLog");
	FMessageLogInitializationOptions InitOptions;
	InitOptions.bShowPages = true;
	InitOptions.bAllowClear = true;
	InitOptions.bShowFilters = true;
	MessageLogModule.RegisterLogListing("Pirate", NSLOCTEXT("PirateSurvivors", "PirateSurvivorsLogLabel", "Pirate Survivors - Main"), InitOptions);
#endif
}

void DeinitPirateMessageLog()
{
#if WITH_EDITOR
	if (FModuleManager::Get().IsModuleLoaded("MessageLog"))
	{
		// Unregister message log
		FMessageLogModule& MessageLogModule = FModuleManager::GetModuleChecked<FMessageLogModule>("MessageLog");
		MessageLogModule.UnregisterLogListing("Pirate");
	}
#endif
}
