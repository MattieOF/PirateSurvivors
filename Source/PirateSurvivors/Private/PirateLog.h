// copyright lololol

#pragma once

#if WITH_EDITOR
	#include "MessageLogModule.h"
#endif

#if !UE_BUILD_SHIPPING
	#include "Engine/Console.h"
#endif

DECLARE_LOG_CATEGORY_EXTERN(LogPirate, Log, All);

#define PIRATE_LOG(fmt, ...) PirateLog(ELogVerbosity::Log, false, true, true, false, __FILE__, __LINE__, __FUNCTION__, nullptr, TEXT(fmt), __VA_ARGS__)
#define PIRATE_LOG_VERBOSE(fmt, ...) PirateLog(ELogVerbosity::Verbose, false, true, true, false, __FILE__, __LINE__, __FUNCTION__, nullptr, TEXT(fmt), __VA_ARGS__)
#define PIRATE_LOG_WARN(fmt, ...) PirateLog(ELogVerbosity::Warning, false, true, true, false, __FILE__, __LINE__, __FUNCTION__, nullptr, TEXT(fmt), __VA_ARGS__)
#define PIRATE_LOG_ERROR(fmt, ...) PirateLog(ELogVerbosity::Error, false, true, true, true, __FILE__, __LINE__, __FUNCTION__, nullptr, TEXT(fmt), __VA_ARGS__)
#define PIRATE_LOG_FATAL(fmt, ...) PirateLog(ELogVerbosity::Fatal, false, true, true, true, __FILE__, __LINE__, __FUNCTION__, nullptr, TEXT(fmt), __VA_ARGS__)

#define PIRATE_LOGC(WorldContext, fmt, ...) PirateLog(ELogVerbosity::Log, true, true, true, false, __FILE__, __LINE__, __FUNCTION__, WorldContext, TEXT(fmt), __VA_ARGS__)
#define PIRATE_LOGC_VERBOSE(WorldContext, fmt, ...) PirateLog(ELogVerbosity::Verbose, true, true, true, false, __FILE__, __LINE__, __FUNCTION__, WorldContext, TEXT(fmt), __VA_ARGS__)
#define PIRATE_LOGC_WARN(WorldContext, fmt, ...) PirateLog(ELogVerbosity::Warning, true, true, true, false, __FILE__, __LINE__, __FUNCTION__, WorldContext, TEXT(fmt), __VA_ARGS__)
#define PIRATE_LOGC_ERROR(WorldContext, fmt, ...) PirateLog(ELogVerbosity::Error, true, true, true, true, __FILE__, __LINE__, __FUNCTION__, WorldContext, TEXT(fmt), __VA_ARGS__)
#define PIRATE_LOGC_FATAL(WorldContext, fmt, ...) PirateLog(ELogVerbosity::Fatal, true, true, true, true, __FILE__, __LINE__, __FUNCTION__, WorldContext, TEXT(fmt), __VA_ARGS__)

#define PIRATE_LOGS(fmt, ...) PirateLog(ELogVerbosity::Log, false, true, true, false, __FILE__, __LINE__, __FUNCTION__, nullptr, fmt, __VA_ARGS__)
#define PIRATE_LOGS_VERBOSE(fmt, ...) PirateLog(ELogVerbosity::Verbose, false, true, true, false, __FILE__, __LINE__, __FUNCTION__, nullptr, fmt, __VA_ARGS__)
#define PIRATE_LOGS_WARN(fmt, ...) PirateLog(ELogVerbosity::Warning, false, true, true, false, __FILE__, __LINE__, __FUNCTION__, nullptr, fmt, __VA_ARGS__)
#define PIRATE_LOGS_ERROR(fmt, ...) PirateLog(ELogVerbosity::Error, false, true, true, true, __FILE__, __LINE__, __FUNCTION__, nullptr, fmt, __VA_ARGS__)
#define PIRATE_LOGS_FATAL(fmt, ...) PirateLog(ELogVerbosity::Fatal, false, true, true, true, __FILE__, __LINE__, __FUNCTION__, nullptr, fmt, __VA_ARGS__)

#define PIRATE_LOGSC(WorldContext, fmt, ...) PirateLog(ELogVerbosity::Log, true, true, true, false, __FILE__, __LINE__, __FUNCTION__, WorldContext, fmt, __VA_ARGS__)
#define PIRATE_LOGSC_VERBOSE(WorldContext, fmt, ...) PirateLog(ELogVerbosity::Verbose, true, true, true, false, __FILE__, __LINE__, __FUNCTION__, WorldContext, fmt, __VA_ARGS__)
#define PIRATE_LOGSC_WARN(WorldContext, fmt, ...) PirateLog(ELogVerbosity::Warning, true, true, true, false, __FILE__, __LINE__, __FUNCTION__, WorldContext, fmt, __VA_ARGS__)
#define PIRATE_LOGSC_ERROR(WorldContext, fmt, ...) PirateLog(ELogVerbosity::Error, true, true, true, true, __FILE__, __LINE__, __FUNCTION__, WorldContext, fmt, __VA_ARGS__)
#define PIRATE_LOGSC_FATAL(WorldContext, fmt, ...) PirateLog(ELogVerbosity::Fatal, true, true, true, true, __FILE__, __LINE__, __FUNCTION__, WorldContext, fmt, __VA_ARGS__)

#define PIRATE_LOG_NOLOC(fmt, ...) PirateLog(ELogVerbosity::Log, false, true, false, false, __FILE__, __LINE__, __FUNCTION__, nullptr, TEXT(fmt), __VA_ARGS__)
#define PIRATE_LOG_VERBOSE_NOLOC(fmt, ...) PirateLog(ELogVerbosity::Verbose, false, true, false, false, __FILE__, __LINE__, __FUNCTION__, nullptr, TEXT(fmt), __VA_ARGS__)
#define PIRATE_LOG_WARN_NOLOC(fmt, ...) PirateLog(ELogVerbosity::Warning, false, true, false, false, __FILE__, __LINE__, __FUNCTION__, nullptr, TEXT(fmt), __VA_ARGS__)
#define PIRATE_LOG_ERROR_NOLOC(fmt, ...) PirateLog(ELogVerbosity::Error, false, true, false, true, __FILE__, __LINE__, __FUNCTION__, nullptr, TEXT(fmt), __VA_ARGS__)
#define PIRATE_LOG_FATAL_NOLOC(fmt, ...) PirateLog(ELogVerbosity::Fatal, false, true, false, true, __FILE__, __LINE__, __FUNCTION__, nullptr, TEXT(fmt), __VA_ARGS__)

#define PIRATE_LOGC_NOLOC(WorldContext, fmt, ...) PirateLog(ELogVerbosity::Log, true, true, false, false, __FILE__, __LINE__, __FUNCTION__, WorldContext, TEXT(fmt), __VA_ARGS__)
#define PIRATE_LOGC_VERBOSE_NOLOC(WorldContext, fmt, ...) PirateLog(ELogVerbosity::Verbose, true, true, false, false, __FILE__, __LINE__, __FUNCTION__, WorldContext, TEXT(fmt), __VA_ARGS__)
#define PIRATE_LOGC_WARN_NOLOC(WorldContext, fmt, ...) PirateLog(ELogVerbosity::Warning, true, true, false, false, __FILE__, __LINE__, __FUNCTION__, WorldContext, TEXT(fmt), __VA_ARGS__)
#define PIRATE_LOGC_ERROR_NOLOC(WorldContext, fmt, ...) PirateLog(ELogVerbosity::Error, true, true, false, true, __FILE__, __LINE__, __FUNCTION__, WorldContext, TEXT(fmt), __VA_ARGS__)
#define PIRATE_LOGC_FATAL_NOLOC(WorldContext, fmt, ...) PirateLog(ELogVerbosity::Fatal, true, true, false, true, __FILE__, __LINE__, __FUNCTION__, WorldContext, TEXT(fmt), __VA_ARGS__)

#define PIRATE_LOGS_NOLOC(fmt, ...) PirateLog(ELogVerbosity::Log, false, true, false, false, __FILE__, __LINE__, __FUNCTION__, nullptr, fmt, __VA_ARGS__)
#define PIRATE_LOGS_VERBOSE_NOLOC(fmt, ...) PirateLog(ELogVerbosity::Verbose, false, true, false, false, __FILE__, __LINE__, __FUNCTION__, nullptr, fmt, __VA_ARGS__)
#define PIRATE_LOGS_WARN_NOLOC(fmt, ...) PirateLog(ELogVerbosity::Warning, false, true, false, false, __FILE__, __LINE__, __FUNCTION__, nullptr, fmt, __VA_ARGS__)
#define PIRATE_LOGS_ERROR_NOLOC(fmt, ...) PirateLog(ELogVerbosity::Error, false, true, false, true, __FILE__, __LINE__, __FUNCTION__, nullptr, fmt, __VA_ARGS__)
#define PIRATE_LOGS_FATAL_NOLOC(fmt, ...) PirateLog(ELogVerbosity::Fatal, false, true, false, true, __FILE__, __LINE__, __FUNCTION__, nullptr, fmt, __VA_ARGS__)

#define PIRATE_LOGSC_NOLOC(WorldContext, fmt, ...) PirateLog(ELogVerbosity::Log, true, true, false, false, __FILE__, __LINE__, __FUNCTION__, WorldContext, fmt, __VA_ARGS__)
#define PIRATE_LOGSC_VERBOSE_NOLOC(WorldContext, fmt, ...) PirateLog(ELogVerbosity::Verbose, true, true, false, false, __FILE__, __LINE__, __FUNCTION__, WorldContext, fmt, __VA_ARGS__)
#define PIRATE_LOGSC_WARN_NOLOC(WorldContext, fmt, ...) PirateLog(ELogVerbosity::Warning, true, true, false, false, __FILE__, __LINE__, __FUNCTION__, WorldContext, fmt, __VA_ARGS__)
#define PIRATE_LOGSC_ERROR_NOLOC(WorldContext, fmt, ...) PirateLog(ELogVerbosity::Error, true, true, false, true, __FILE__, __LINE__, __FUNCTION__, WorldContext, fmt, __VA_ARGS__)
#define PIRATE_LOGSC_FATAL_NOLOC(WorldContext, fmt, ...) PirateLog(ELogVerbosity::Fatal, true, true, false, true, __FILE__, __LINE__, __FUNCTION__, WorldContext, fmt, __VA_ARGS__)

void InitPirateMessageLog();
void DeinitPirateMessageLog();

template <typename FmtType, typename... Types>
FORCEINLINE void PirateLog(const uint8 LogVerbosity, const bool bLogToConsole, const bool bLogToMsgLog, const bool bShowLocation,
                      const bool bShowMsgLog, const char* File, const int Line, const char* Function, UObject* WorldContext,
                      const FmtType& Fmt, Types... Args)
{
	static_assert(TIsArrayOrRefOfTypeByPredicate<FmtType, TIsCharEncodingCompatibleWithTCHAR>::Value, "Formatting string must be a TCHAR array.");
	static_assert(TAnd<TIsValidVariadicFunctionArg<Types>...>::Value, "Invalid argument(s) passed to PirateLog");
	
	FString Msg = FString::Printf(Fmt, Args...);
	if (bShowLocation)
	{
		Msg = FString::Printf(TEXT("%s(%d): %s: %s"), ANSI_TO_TCHAR(File), Line, ANSI_TO_TCHAR(Function), *Msg);
	}

	switch (LogVerbosity)
	{
	case ELogVerbosity::Verbose:
		UE_LOG(LogPirate, Verbose, TEXT("%s"), *Msg);
		break;
	case ELogVerbosity::Log:
	default:
		UE_LOG(LogPirate, Log, TEXT("%s"), *Msg);
		break;
	case ELogVerbosity::Warning:
		UE_LOG(LogPirate, Warning, TEXT("%s"), *Msg);
		break;
	case ELogVerbosity::Error:
		UE_LOG(LogPirate, Error, TEXT("%s"), *Msg);
		break;
	case ELogVerbosity::Fatal:
		UE_LOG(LogPirate, Fatal, TEXT("%s"), *Msg);
		break;
	}
	
#if !UE_BUILD_SHIPPING
	if (bLogToConsole)
	{
		WorldContext->GetWorld()->GetGameViewport()->ViewportConsole->OutputText(Msg);
	}
#endif

#if WITH_EDITOR
	if (bLogToMsgLog)
	{
		switch (LogVerbosity)
		{
		case ELogVerbosity::Verbose:
		case ELogVerbosity::Log:
		default:
			FMessageLog("Pirate").Info(FText::FromString(Msg));
			break;
		case ELogVerbosity::Warning:
			FMessageLog("Pirate").Warning(FText::FromString(Msg));
			break;
		case ELogVerbosity::Error:
		case ELogVerbosity::Fatal:
			FMessageLog("Pirate").Error(FText::FromString(Msg));
			break;
		}

		if (bShowMsgLog)
		{
			FMessageLogModule& MessageLogModule = FModuleManager::LoadModuleChecked<FMessageLogModule>("MessageLog");
			MessageLogModule.OpenMessageLog("Pirate");
		}
	}
#endif
}
