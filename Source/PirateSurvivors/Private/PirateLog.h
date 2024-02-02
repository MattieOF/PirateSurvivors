// copyright lololol

#pragma once

#if WITH_EDITOR
	#include "MessageLogModule.h"
#endif

DECLARE_LOG_CATEGORY_EXTERN(LogPirate, Log, All);

#if WITH_EDITOR
	#define PIRATE_LOG(x) FMessageLog("Pirate").Info(FText::FromString(x));
	#define PIRATE_LOG_WARN(x) FMessageLog("Pirate").Warning(FText::FromString(x));
	#define PIRATE_LOG_ERROR(x) do { FMessageLog("Pirate").Error(FText::FromString(x));\
							 { FMessageLogModule& MessageLogModule = FModuleManager::LoadModuleChecked<FMessageLogModule>("MessageLog");\
							 MessageLogModule.OpenMessageLog("Pirate");} } while(false)
#else
	#define PIRATE_LOG(x) UE_LOG(LogPirate, Log, TEXT(x));
	#define PIRATE_LOG_WARN(x) UE_LOG(LogPirate, Warning, TEXT(x));
	#define PIRATE_LOG_ERROR(x) UE_LOG(LogPirate, Error, TEXT(x));
#endif
