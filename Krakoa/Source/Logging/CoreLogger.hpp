#pragma once

#include "../Core/EngineMacros.hpp"
#include "../Core/PointerTypes.hpp"

#define KLOG_OPT_DBG_STR 1
#include <Utility/Logging/kLogging.hpp>

#include <memory>


namespace krakoa
{
	class KRAKOA_API CoreLogger
	{
	public:
		// Engine side
		static void CoreInit(const std::string_view& initMsg);
		static klib::kLogs::Logging& GetLogger();
		static void ShutDown();
		
	private:
		static Solo_Ptr<klib::kLogs::Logging> pCoreLogger;
	};
}

// Initializer
#define KRK_LOG_LVL_DBG                                            ::klib::kLogs::LogLevel::DBG
#define KRK_LOG_LVL_NRM                                            ::klib::kLogs::LogLevel::NRM
#define KRK_LOG_LVL_INF                                            ::klib::kLogs::LogLevel::INF
#define KRK_LOG_LVL_WRN                                            ::klib::kLogs::LogLevel::WRN
#define KRK_LOG_LVL_ERR                                            ::klib::kLogs::LogLevel::ERR


// ENGINE SIDE Log Macros
#define KRK_INIT_LOGS(msg)                                          ::krakoa::CoreLogger::CoreInit(msg);
#define KRK_SET_LOG_MIN(minLvl)                                     ::krakoa::CoreLogger::GetLogger().SetMinimumLoggingLevel(minLvl);
#define KRK_GET_LOG_PATH()                                          ::krakoa::CoreLogger::GetLogger().GetOutputPath();
#define KRK_TOGGLE_LOGGING()                                        ::krakoa::CoreLogger::GetLogger().ToggleLoggingEnabled();
#define KRK_DISABLE_FILE_OUTPUT()                                   ::krakoa::CoreLogger::GetLogger().SetCacheMode(true);
#define KRK_ENABLE_FILE_OUTPUT()                                    ::krakoa::CoreLogger::GetLogger().SetCacheMode(false);
#define KRK_CHANGE_LOGS_DESTINATION(destination)                    ::krakoa::CoreLogger::GetLogger().ChangeOutputDirectory(destination);
#define KRK_CHANGE_LOGS_FILENAME(name)                              ::krakoa::CoreLogger::GetLogger().ChangeFilename(name);
#define KRK_LOG_RAW(msg)                                            ::krakoa::CoreLogger::GetLogger().AddRaw(msg);
#define KRK_DBG(msg)                                                ::krakoa::CoreLogger::GetLogger().AddEntry(KRK_LOG_LVL_DBG, ::klib::kLogs::LogMessage(msg));
#define KRK_NRM(msg)                                                ::krakoa::CoreLogger::GetLogger().AddEntry(KRK_LOG_LVL_NRM, ::klib::kLogs::LogMessage(msg));
#define KRK_INF(msg)                                                ::krakoa::CoreLogger::GetLogger().AddEntry(KRK_LOG_LVL_INF, ::klib::kLogs::LogMessage(msg));
#define KRK_WRN(msg)                                                ::krakoa::CoreLogger::GetLogger().AddEntry(KRK_LOG_LVL_WRN, ::klib::kLogs::LogMessage(msg, SOURCE_INFO()));
#define KRK_ERR(msg)                                                ::krakoa::CoreLogger::GetLogger().AddEntry(KRK_LOG_LVL_ERR, ::klib::kLogs::LogMessage(msg, SOURCE_INFO()));
#define KRK_BANNER(msg, descriptor, frontPad, backPad, count)       ::krakoa::CoreLogger::GetLogger().AddBanner(msg, descriptor, frontPad, backPad, count);
#define KRK_SUSPEND()                                               ::krakoa::CoreLogger::GetLogger().SuspendFileLogging();
#define KRK_RESUME()                                                ::krakoa::CoreLogger::GetLogger().ResumeFileLogging();
#define KRK_LOG_END()                                               ::krakoa::CoreLogger::ShutDown();
#define KRK_LOG_GET_LAST()                                          ::krakoa::CoreLogger::GetLogger().GetLastCachedEntry()
#define KRK_LOG_ERASE_PREV(numOfPrevEntries)                        ::krakoa::CoreLogger::GetLogger().ErasePrevious(numOfPrevEntries)
#define KRK_LOG_CLEAR()                                             ::krakoa::CoreLogger::GetLogger().ClearCache();

