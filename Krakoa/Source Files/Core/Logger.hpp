#pragma once

#include <Core/EngineCore.hpp>

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
		// Engine side
		static void CoreInit();
		static klib::kLogs::Logging& GetCoreLogger()            { return *coreLogger; }

		// client side
		static void ClientInit();
		static klib::kLogs::Logging& GetClientLogger()          { return *clientLogger; }

	private:
		static std::unique_ptr<klib::kLogs::Logging> coreLogger;
		static std::unique_ptr<klib::kLogs::Logging> clientLogger;
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


// ENGINE SIDE Log Macros
#define KRK_INIT_LOGS()                             ::krakoa::Logger::CoreInit();
#define KRK_SET_LOG_MIN(minLvl)                     ::krakoa::Logger::GetCoreLogger().SetMinimumLoggingLevel(minLvl);
#define KRK_TOGGLE_LOGGING()                        ::krakoa::Logger::GetCoreLogger().ToggleLoggingEnabled();
#define KRK_DISABLE_FILE_OUTPUT()                   ::krakoa::Logger::GetCoreLogger().SetCacheMode(true);
#define KRK_ENABLE_FILE_OUTPUT()                    ::krakoa::Logger::GetCoreLogger().SetCacheMode(false);
#define KRK_CHANGE_LOGS_DESTINATION(destination)    ::krakoa::Logger::GetCoreLogger().ChangeOutputDirectory(destination);
#define KRK_CHANGE_LOGS_FILENAME(name)              ::krakoa::Logger::GetCoreLogger().ChangeFilename(name);
#define KRK_DBUG(msg)                               ::krakoa::Logger::GetCoreLogger().AddEntry(msg, LOG_LVL_DBUG);
#define KRK_NORM(msg)                               ::krakoa::Logger::GetCoreLogger().AddEntry(msg, LOG_LVL_NORM);
#define KRK_INFO(msg)                               ::krakoa::Logger::GetCoreLogger().AddEntry(msg, LOG_LVL_INFO);
#define KRK_WARN(msg)                               ::krakoa::Logger::GetCoreLogger().AddEntry(msg, LOG_LVL_WARN);
#define KRK_ERRR(msg)                               ::krakoa::Logger::GetCoreLogger().AddEntry(msg, LOG_LVL_ERRR, __FILE__, __LINE__);
#define KRK_FATAL(msg)                              ::krakoa::Logger::GetCoreLogger().OutputToFatalFile(msg, __FILE__, __LINE__);
#define KRK_BANNER(banner, category)                ::krakoa::Logger::GetCoreLogger().AddEntryBanner(banner, category);
#define KRK_SUSPEND()                               ::krakoa::Logger::GetCoreLogger().SuspendFileLogging();
#define KRK_RESUME()                                ::krakoa::Logger::GetCoreLogger().ResumeFileLogging();
#define KRK_LOG_END()                                   ::krakoa::Logger::GetCoreLogger().FinalOutput();
#define KRK_LOG_GET_LAST()                              ::krakoa::Logger::GetCoreLogger().GetLastCachedEntry();
#define KRK_LOG_ERASE_PREV(numOfPrevEntries)            ::krakoa::Logger::GetCoreLogger().ErasePreviousCacheEntries(numOfPrevEntries);
#define KRK_LOG_CLEAR()                                 ::krakoa::Logger::GetCoreLogger().ClearCache();

// CLIENT SIDE Log Macros
#define INIT_LOGS()                                 ::krakoa::Logger::ClientInit();
#define SET LOG_NAME(name)                              ::krakoa::Logger::GetClientLogger().SetName(name);
#define SET_LOG_MIN(minLvl)                         ::krakoa::Logger::GetClientLogger().SetMinimumLoggingLevel(minLvl);
#define TOGGLE_LOGGING()                            ::krakoa::Logger::GetClientLogger().ToggleLoggingEnabled();
#define DISABLE_LOGFILE_OUTPUT()                       ::krakoa::Logger::GetClientLogger().SetCacheMode(true);
#define ENABLE_LOGFILE_OUTPUT()                        ::krakoa::Logger::GetClientLogger().SetCacheMode(false);
#define CHANGE_LOGS_DESTINATION(destination)        ::krakoa::Logger::GetClientLogger().ChangeOutputDirectory(destination);
#define CHANGE_LOGS_FILENAME(name)                  ::krakoa::Logger::GetClientLogger().ChangeFilename(name);
#define DBUG(msg)                                   ::krakoa::Logger::GetClientLogger().AddEntry(msg, LOG_LVL_DBUG);
#define NORM(msg)                                   ::krakoa::Logger::GetClientLogger().AddEntry(msg, LOG_LVL_NORM);
#define INFO(msg)                                   ::krakoa::Logger::GetClientLogger().AddEntry(msg, LOG_LVL_INFO);
#define WARN(msg)                                   ::krakoa::Logger::GetClientLogger().AddEntry(msg, LOG_LVL_WARN);
#define ERRR(msg)                                   ::krakoa::Logger::GetClientLogger().AddEntry(msg, LOG_LVL_ERRR, __FILE__, __LINE__);
#define FATAL(msg)                                  ::krakoa::Logger::GetClientLogger().OutputToFatalFile(msg, __FILE__, __LINE__);
#define BANNER(banner, category)                    ::krakoa::Logger::GetClientLogger().AddEntryBanner(banner, category);
#define LOG_SUSPEND()                                   ::krakoa::Logger::GetClientLogger().SuspendFileLogging();
#define LOG_RESUME()                                    ::krakoa::Logger::GetClientLogger().ResumeFileLogging();
#define LOG_END()                                   ::krakoa::Logger::GetClientLogger().FinalOutput();
#define LOG_GET_LAST()                                  ::krakoa::Logger::GetClientLogger().GetLastCachedEntry();
#define LOG_ERASE_PREV(numOfPrevEntries)                ::krakoa::Logger::GetClientLogger().ErasePreviousCacheEntries(numOfPrevEntries);
#define LOG_CLEAR()                                     ::krakoa::Logger::GetClientLogger().ClearCache();
