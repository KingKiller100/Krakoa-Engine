#pragma once

#include "../EngineCore.hpp"

#include <Utility/Logging/kLogging_Class.hpp>

#include <memory>

namespace krakoa
{
	class KRAKOA_API CoreLogger
	{
	public:
		// Engine side
		static void CoreInit();
		static klib::kLogs::Logging& GetCoreLogger() { return *pCoreLogger; }

	private:
		static std::unique_ptr<klib::kLogs::Logging> pCoreLogger;
	};
}

// Initializer
#define LOG_LVL_DBUG                                    ::klib::kLogs::LLevel::DBUG
#define LOG_LVL_NORM                                    ::klib::kLogs::LLevel::NORM
#define LOG_LVL_INFO                                    ::klib::kLogs::LLevel::INFO
#define LOG_LVL_WARN                                    ::klib::kLogs::LLevel::WARN
#define LOG_LVL_ERRR                                    ::klib::kLogs::LLevel::ERRR


// ENGINE SIDE Log Macros
#define KRK_INIT_LOGS()                                 ::krakoa::CoreLogger::CoreInit();
#define KRK_SET_LOG_MIN(minLvl)                         ::krakoa::CoreLogger::GetCoreLogger().SetMinimumLoggingLevel(minLvl);
#define KRK_TOGGLE_LOGGING()                            ::krakoa::CoreLogger::GetCoreLogger().ToggleLoggingEnabled();
#define KRK_DISABLE_FILE_OUTPUT()                       ::krakoa::CoreLogger::GetCoreLogger().SetCacheMode(true);
#define KRK_ENABLE_FILE_OUTPUT()                        ::krakoa::CoreLogger::GetCoreLogger().SetCacheMode(false);
#define KRK_CHANGE_LOGS_DESTINATION(destination)        ::krakoa::CoreLogger::GetCoreLogger().ChangeOutputDirectory(destination);
#define KRK_CHANGE_LOGS_FILENAME(name)                  ::krakoa::CoreLogger::GetCoreLogger().ChangeFilename(name);
#define KRK_DBUG(msg)                                   ::krakoa::CoreLogger::GetCoreLogger().AddEntry(msg, LOG_LVL_DBUG);
#define KRK_NORM(msg)                                   ::krakoa::CoreLogger::GetCoreLogger().AddEntry(msg, LOG_LVL_NORM);
#define KRK_INFO(msg)                                   ::krakoa::CoreLogger::GetCoreLogger().AddEntry(msg, LOG_LVL_INFO);
#define KRK_WARN(msg)                                   ::krakoa::CoreLogger::GetCoreLogger().AddEntry(msg, LOG_LVL_WARN);
#define KRK_ERRR(msg)                                   ::krakoa::CoreLogger::GetCoreLogger().AddEntry(msg, LOG_LVL_ERRR, __FILE__, __LINE__);
#define KRK_BANNER(banner, category)                    ::krakoa::CoreLogger::GetCoreLogger().AddEntryBanner(banner, category);
#define KRK_SUSPEND()                                   ::krakoa::CoreLogger::GetCoreLogger().SuspendFileLogging();
#define KRK_RESUME()                                    ::krakoa::CoreLogger::GetCoreLogger().ResumeFileLogging();
#define KRK_LOG_END()                                   ::krakoa::CoreLogger::GetCoreLogger().FinalOutput();
#define KRK_LOG_GET_LAST()                              ::krakoa::CoreLogger::GetCoreLogger().GetLastCachedEntry()
#define KRK_LOG_ERASE_PREV(numOfPrevEntries)            ::krakoa::CoreLogger::GetCoreLogger().ErasePreviousCacheEntries(numOfPrevEntries)
#define KRK_LOG_CLEAR()                                 ::krakoa::CoreLogger::GetCoreLogger().ClearCache();

#ifndef KRAKOA_RELEASE
#	include <Utility/Debug Helper/kAssert.hpp>
#	define KRK_FATAL(condition, msg)                                  kAssert(condition, msg)
#else
#	define KRK_FATAL(condition, msg)                                  if ( !condition ) ::krakoa::CoreLogger::GetCoreLogger().OutputToFatalFile(msg, __FILE__, __LINE__)
#endif // !KRAKOA_RELEASE
