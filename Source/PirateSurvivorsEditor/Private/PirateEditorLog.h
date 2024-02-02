// copyright lololol

#pragma once

#include "MessageLogModule.h"
#include "Logging/MessageLog.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPortalRemake, Log, All);

#define PIRATED_LOG(x) FMessageLog("PiratEd").Info(FText::FromString(x));
#define PIRATED_LOG_WARN(x) FMessageLog("PiratEd").Warning(FText::FromString(x));
#define PIRATED_LOG_ERROR(x) do { FMessageLog("PiratEd").Error(FText::FromString(x));\
						{ FMessageLogModule& MessageLogModule = FModuleManager::LoadModuleChecked<FMessageLogModule>("MessageLog");\
						MessageLogModule.OpenMessageLog("PiratEd");} } while(false)
