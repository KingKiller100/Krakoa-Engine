#pragma once

#include "../Core/EngineMacros.hpp"

#include <Utility/Logging/kLogging.hpp>

#include <memory>

namespace krakoa
{
	class Logger
	{
	public:
		// client side
		static void ClientInit(const char* name);
		static klib::kLogs::Logging& GetLogger();

		static void ToggleConsoleLogging();
		static void ToggleFileLogging();

	private:
		static std::unique_ptr<klib::kLogs::Logging> pClientLogger;
	};
}

#define LOG_LVL_DBG                                            ::klib::kLogs::LogLevel::DBG
#define LOG_LVL_NRM                                            ::klib::kLogs::LogLevel::NRM
#define LOG_LVL_INF                                            ::klib::kLogs::LogLevel::INF
#define LOG_LVL_WRN                                            ::klib::kLogs::LogLevel::WRN
#define LOG_LVL_ERR                                            ::klib::kLogs::LogLevel::ERR

// CLIENT SIDE Log Macros
#define INIT_LOGS(name)                                         ::krakoa::Logger::ClientInit(name);
#define SET_LOG_NAME(name)                                      ::krakoa::Logger::GetLogger().SetName(name);
#define SET_LOG_MIN(minLvl)                                     ::krakoa::Logger::GetLogger().SetMinimumLoggingLevel(minLvl);
#define GET_LOG_PATH()                                          ::krakoa::Logger::GetLogger().GetFile().GetPath();
#define TOGGLE_LOGGING()                                        ::krakoa::Logger::GetLogger().ToggleLoggingEnabled();
#define TOGGLE_CONSOLE_LOGGING()                                ::krakoa::Logger::ToggleConsoleLogging();
#define DISABLE_LOGFILE_OUTPUT()                                ::krakoa::Logger::GetLogger().SetCacheMode(true);
#define ENABLE_LOGFILE_OUTPUT()                                 ::krakoa::Logger::GetLogger().SetCacheMode(false);
#define CHANGE_LOGS_DESTINATION(destination)                    ::krakoa::Logger::GetLogger().GetFile().SetDirectory(destination);
#define CHANGE_LOGS_FILENAME(name)                              ::krakoa::Logger::GetLogger().GetFile().SetFileName(name);
#define VERBATIM(msg)                                           ::krakoa::Logger::GetLogger().AddRaw(msg);
#define DBUG(msg)                                               ::krakoa::Logger::GetLogger().AddEntry(LOG_LVL_DBG, ::klib::kLogs::LogMessage(msg));
#define NORM(msg)                                               ::krakoa::Logger::GetLogger().AddEntry(LOG_LVL_NRM, ::klib::kLogs::LogMessage(msg));
#define INFO(msg)                                               ::krakoa::Logger::GetLogger().AddEntry(LOG_LVL_INF, ::klib::kLogs::LogMessage(msg));
#define WARN(msg)                                               ::krakoa::Logger::GetLogger().AddEntry(LOG_LVL_WRN, ::klib::kLogs::LogMessage(msg));
#define ERRR(msg)                                               ::krakoa::Logger::GetLogger().AddEntry(LOG_LVL_ERR, ::klib::kLogs::LogMessage(msg, SOURCE_INFO()));
#define BANNER(msg, descriptor, frontPad, backPad, count)       ::krakoa::Logger::GetLogger().AddBanner(msg, descriptor, frontPad, backPad, count);
#define LOG_SUSPEND()                                           ::krakoa::Logger::GetLogger().GetFile().Close(false);
#define LOG_RESUME()                                            ::krakoa::Logger::GetLogger().GetFile().Open();
#define LOG_END()                                               ::krakoa::Logger::GetLogger().FinalOutput();
#define LOG_GET_LAST()                                          ::krakoa::Logger::GetLogger().GetLastCachedEntry();
#define LOG_ERASE_PREV(numOfPrevEntries)                        ::krakoa::Logger::GetLogger().ErasePrevious(numOfPrevEntries);
#define LOG_CLEAR()                                             ::krakoa::Logger::GetLogger().ClearCache();
