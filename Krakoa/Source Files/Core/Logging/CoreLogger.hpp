﻿#pragma once

#include "../../EngineMacros.hpp"

#include <Utility/Logging/kLogging.hpp>

#include <memory>

namespace krakoa
{
	class KRAKOA_API CoreLogger
	{
	public:
		// Engine side
		static void CoreInit(const std::string_view& initMsg);
		static klib::kLogs::Logging& GetCoreLogger();

	private:
		static Solo_Ptr<klib::kLogs::Logging> pCoreLogger;
	};
}

// Initializer
#define KRK_LOG_LVL_DBUG                                            ::klib::kLogs::LogLevel::DBUG
#define KRK_LOG_LVL_NORM                                            ::klib::kLogs::LogLevel::NORM
#define KRK_LOG_LVL_INFO                                            ::klib::kLogs::LogLevel::INFO
#define KRK_LOG_LVL_WARN                                            ::klib::kLogs::LogLevel::WARN
#define KRK_LOG_LVL_ERRR                                            ::klib::kLogs::LogLevel::ERRR


// ENGINE SIDE Log Macros
#define KRK_INIT_LOGS(msg)                                          ::krakoa::CoreLogger::CoreInit(msg);
#define KRK_SET_LOG_MIN(minLvl)                                     ::krakoa::CoreLogger::GetCoreLogger().SetMinimumLoggingLevel(minLvl);
#define KRK_GET_LOG_PATH()                                          ::krakoa::CoreLogger::GetCoreLogger().GetOutputPath();
#define KRK_TOGGLE_LOGGING()                                        ::krakoa::CoreLogger::GetCoreLogger().ToggleLoggingEnabled();
#define KRK_DISABLE_FILE_OUTPUT()                                   ::krakoa::CoreLogger::GetCoreLogger().SetCacheMode(true);
#define KRK_ENABLE_FILE_OUTPUT()                                    ::krakoa::CoreLogger::GetCoreLogger().SetCacheMode(false);
#define KRK_CHANGE_LOGS_DESTINATION(destination)                    ::krakoa::CoreLogger::GetCoreLogger().ChangeOutputDirectory(destination);
#define KRK_CHANGE_LOGS_FILENAME(name)                              ::krakoa::CoreLogger::GetCoreLogger().ChangeFilename(name);
#define KRK_VBAT(msg)                                               ::krakoa::CoreLogger::GetCoreLogger().AddVerbatim(msg);
#define KRK_DBUG(msg)                                               ::krakoa::CoreLogger::GetCoreLogger().AddEntry(KRK_LOG_LVL_DBUG, ::klib::kLogs::LogMessage(msg));
#define KRK_NORM(msg)                                               ::krakoa::CoreLogger::GetCoreLogger().AddEntry(KRK_LOG_LVL_NORM, ::klib::kLogs::LogMessage(msg));
#define KRK_INFO(msg)                                               ::krakoa::CoreLogger::GetCoreLogger().AddEntry(KRK_LOG_LVL_INFO, ::klib::kLogs::LogMessage(msg));
#define KRK_WARN(msg)                                               ::krakoa::CoreLogger::GetCoreLogger().AddEntry(KRK_LOG_LVL_WARN, ::klib::kLogs::LogMessage(msg));
#define KRK_ERRR(msg)                                               ::krakoa::CoreLogger::GetCoreLogger().AddEntry(KRK_LOG_LVL_ERRR, ::klib::kLogs::LogMessage(msg, SOURCE_INFO()));
#define KRK_BANNER(msg, descriptor, frontPad, backPad, count)       ::krakoa::CoreLogger::GetCoreLogger().AddBanner(msg, descriptor, frontPad, backPad, count);
#define KRK_SUSPEND()                                               ::krakoa::CoreLogger::GetCoreLogger().SuspendFileLogging();
#define KRK_RESUME()                                                ::krakoa::CoreLogger::GetCoreLogger().ResumeFileLogging();
#define KRK_LOG_END()                                               ::krakoa::CoreLogger::GetCoreLogger().FinalOutput();
#define KRK_LOG_GET_LAST()                                          ::krakoa::CoreLogger::GetCoreLogger().GetLastCachedEntry()
#define KRK_LOG_ERASE_PREV(numOfPrevEntries)                        ::krakoa::CoreLogger::GetCoreLogger().ErasePrevious(numOfPrevEntries)
#define KRK_LOG_CLEAR()                                             ::krakoa::CoreLogger::GetCoreLogger().ClearCache();

