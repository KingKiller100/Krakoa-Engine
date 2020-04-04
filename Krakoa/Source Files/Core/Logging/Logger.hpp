#pragma once

#include "../EngineCore.hpp"

#include <Utility/Logging/kLogging_Class.hpp>

#include <memory>

namespace krakoa
{
#if defined (_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable:4251)
		EXPIMP_TEMPLATE template class KRAKOA_API std::unique_ptr<klib::kLogs::Logging, std::default_delete<klib::kLogs::Logging>>;

	class KRAKOA_API Logger
	{
	public:
		// client side
		static void ClientInit(const char * name);
		static klib::kLogs::Logging& GetClientLogger()          { return *pClientLogger; }

	private:
		static std::unique_ptr<klib::kLogs::Logging> pClientLogger;
	};
#	pragma warning(pop)
#endif
}

// Initializer
#define LOG_LVL_DBUG                                    ::klib::kLogs::LLevel::DBUG
#define LOG_LVL_NORM                                    ::klib::kLogs::LLevel::NORM
#define LOG_LVL_INFO                                    ::klib::kLogs::LLevel::INFO
#define LOG_LVL_WARN                                    ::klib::kLogs::LLevel::WARN
#define LOG_LVL_ERRR                                    ::klib::kLogs::LLevel::ERRR

// CLIENT SIDE Log Macros
#define INIT_LOGS(name)                                 ::krakoa::Logger::ClientInit(name);
#define SET LOG_NAME(name)                              ::krakoa::Logger::GetClientLogger().SetName(name);
#define SET_LOG_MIN(minLvl)                             ::krakoa::Logger::GetClientLogger().SetMinimumLoggingLevel(minLvl);
#define TOGGLE_LOGGING()                                ::krakoa::Logger::GetClientLogger().ToggleLoggingEnabled();
#define TOGGLE_SUBSYSTEM_LOGGING()                      ::krakoa::Logger::GetClientLogger().ToggleSubSystemEnabled();
#define DISABLE_LOGFILE_OUTPUT()                        ::krakoa::Logger::GetClientLogger().SetCacheMode(true);
#define ENABLE_LOGFILE_OUTPUT()                         ::krakoa::Logger::GetClientLogger().SetCacheMode(false);
#define CHANGE_LOGS_DESTINATION(destination)            ::krakoa::Logger::GetClientLogger().ChangeOutputDirectory(destination);
#define CHANGE_LOGS_FILENAME(name)                      ::krakoa::Logger::GetClientLogger().ChangeFilename(name);
#define DBUG(msg)                                       ::krakoa::Logger::GetClientLogger().AddEntry(msg, LOG_LVL_DBUG);
#define NORM(msg)                                       ::krakoa::Logger::GetClientLogger().AddEntry(msg, LOG_LVL_NORM);
#define INFO(msg)                                       ::krakoa::Logger::GetClientLogger().AddEntry(msg, LOG_LVL_INFO);
#define WARN(msg)                                       ::krakoa::Logger::GetClientLogger().AddEntry(msg, LOG_LVL_WARN);
#define ERRR(msg)                                       ::krakoa::Logger::GetClientLogger().AddEntry(msg, LOG_LVL_ERRR, __FILE__, __LINE__);
#define BANNER(banner, category)                        ::krakoa::Logger::GetClientLogger().AddEntryBanner(banner, category);
#define LOG_SUSPEND()                                   ::krakoa::Logger::GetClientLogger().SuspendFileLogging();
#define LOG_RESUME()                                    ::krakoa::Logger::GetClientLogger().ResumeFileLogging();
#define LOG_END()                                       ::krakoa::Logger::GetClientLogger().FinalOutput();
#define LOG_GET_LAST()                                  ::krakoa::Logger::GetClientLogger().GetLastCachedEntry();
#define LOG_ERASE_PREV(numOfPrevEntries)                ::krakoa::Logger::GetClientLogger().ErasePreviousCacheEntries(numOfPrevEntries);
#define LOG_CLEAR()                                     ::krakoa::Logger::GetClientLogger().ClearCache();


#ifndef KRAKOA_RELEASE
#	include <Utility/Debug Helper/kAssert.hpp>
#	define FATAL(condition, msg)                                  kAssert(condition, msg);
#else
#	define FATAL(condition, msg)                                  if ( !condition ) ::krakoa::Logger::GetClientLogger().OutputToFatalFile(msg, __FILE__, __LINE__);
#endif // !KRAKOA_RELEASE