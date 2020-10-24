#pragma once

#include "../../EngineMacros.hpp"

#include <Utility/Logging/kLogging.hpp>

#include <memory>

namespace krakoa
{
	class Logger
	{
	public:
		// client side
		static void ClientInit(const char * name);
		static klib::kLogs::Logging& GetLogger();

	private:
		static std::unique_ptr<klib::kLogs::Logging> pClientLogger;
	};
}

#define LOG_LVL_DBUG                                            ::klib::kLogs::LogLevel::DBUG
#define LOG_LVL_NORM                                            ::klib::kLogs::LogLevel::NORM
#define LOG_LVL_INFO                                            ::klib::kLogs::LogLevel::INFO
#define LOG_LVL_WARN                                            ::klib::kLogs::LogLevel::WARN
#define LOG_LVL_ERRR                                            ::klib::kLogs::LogLevel::ERRR
														        
// CLIENT SIDE Log Macros								        
#define INIT_LOGS(name)                                         ::krakoa::Logger::ClientInit(name);
#define SET_LOG_NAME(name)                                      ::krakoa::Logger::GetLogger().SetName(name);
#define SET_LOG_MIN(minLvl)                                     ::krakoa::Logger::GetLogger().SetMinimumLoggingLevel(minLvl);
#define GET_LOG_PATH()                                          ::krakoa::Logger::GetLogger().GetOutputPath();
#define TOGGLE_LOGGING()                                        ::krakoa::Logger::GetLogger().ToggleLoggingEnabled();
#define TOGGLE_SUBSYSTEM_LOGGING()                              ::krakoa::Logger::GetLogger().ToggleConsoleEnabled();
#define DISABLE_LOGFILE_OUTPUT()                                ::krakoa::Logger::GetLogger().SetCacheMode(true);
#define ENABLE_LOGFILE_OUTPUT()                                 ::krakoa::Logger::GetLogger().SetCacheMode(false);
#define CHANGE_LOGS_DESTINATION(destination)                    ::krakoa::Logger::GetLogger().ChangeOutputDirectory(destination);
#define CHANGE_LOGS_FILENAME(name)                              ::krakoa::Logger::GetLogger().ChangeFilename(name);
#define VERBATIM(msg)                                           ::krakoa::Logger::GetLogger().AddVerbatim(msg);
#define DBUG(msg)                                               ::krakoa::Logger::GetLogger().AddEntry(LOG_LVL_DBUG, ::klib::kLogs::LogMessage(msg));
#define NORM(msg)                                               ::krakoa::Logger::GetLogger().AddEntry(LOG_LVL_NORM, ::klib::kLogs::LogMessage(msg));
#define INFO(msg)                                               ::krakoa::Logger::GetLogger().AddEntry(LOG_LVL_INFO, ::klib::kLogs::LogMessage(msg));
#define WARN(msg)                                               ::krakoa::Logger::GetLogger().AddEntry(LOG_LVL_WARN, ::klib::kLogs::LogMessage(msg));
#define ERRR(msg)                                               ::krakoa::Logger::GetLogger().AddEntry(LOG_LVL_ERRR, ::klib::kLogs::LogMessage(msg, __FILE__, __LINE__));
#define BANNER(msg, descriptor, frontPad, backPad, count)       ::krakoa::Logger::GetLogger().AddBanner(msg, descriptor, frontPad, backPad, count);
#define LOG_SUSPEND()                                           ::krakoa::Logger::GetLogger().SuspendFileLogging();
#define LOG_RESUME()                                            ::krakoa::Logger::GetLogger().ResumeFileLogging();
#define LOG_END()                                               ::krakoa::Logger::GetLogger().FinalOutput();
#define LOG_GET_LAST()                                          ::krakoa::Logger::GetLogger().GetLastCachedEntry();
#define LOG_ERASE_PREV(numOfPrevEntries)                        ::krakoa::Logger::GetLogger().ErasePrevious(numOfPrevEntries);
#define LOG_CLEAR()                                             ::krakoa::Logger::GetLogger().ClearCache();


#ifdef KRAKOA_DEBUG
#	include <Utility/Debug Helper/kAssert.hpp>
#	define FATAL(condition, msg)                                kAssert((condition), msg);
#else
#	define FATAL(condition, msg)                                if ( !(condition) ) ::krakoa::Logger::GetLogger().OutputToFatalFile(::klib::kLogs::LogMessage(msg, __FILE__, __LINE__));
#endif // !KRAKOA_RELEASE