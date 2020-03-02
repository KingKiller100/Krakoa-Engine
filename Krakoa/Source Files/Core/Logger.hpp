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
#	pragma warning(pop)
#endif

	using namespace klib::kLogs;

	class KRAKOA_API Logger
	{
	public:
		// Engine side
		static void CoreInit();
		inline static Logging& GetCoreLogger()            { return *coreLogger; }

		// client side
		static void ClientInit();
		inline static Logging& GetClientLogger()          { return *clientLogger; }

	private:
		static std::unique_ptr<Logging> coreLogger;
		static std::unique_ptr<Logging> clientLogger;
	};
}

// Initializer
#define LVL_DBUG                                   ::krakoa::LLevel::DBUG
#define LVL_NORM                                   ::krakoa::LLevel::NORM
#define LVL_INFO                                   ::krakoa::LLevel::INFO
#define LVL_WARN                                   ::krakoa::LLevel::WARN
#define LVL_ERRR                                   ::krakoa::LLevel::ERRR
#define LVL_FATL                                   ::krakoa::LLevel::FATL


// ENGINE SIDE Log Macros
#define KRK_INIT_LOGS()                             ::krakoa::Logger::CoreInit();
#define KRK_SET_MIN(minLvl)                         ::krakoa::Logger::GetCoreLogger().SetMinimumLoggingLevel(minLvl);
#define KRK_TOGGLE_LOGGING()                        ::krakoa::Logger::GetCoreLogger().ToggleLoggingEnabled();
#define KRK_DISABLE_FILE_OUTPUT()                   ::krakoa::Logger::GetCoreLogger().SetCacheMode(true);
#define KRK_ENABLE_FILE_OUTPUT()                    ::krakoa::Logger::GetCoreLogger().SetCacheMode(false);
#define KRK_CHANGE_LOGS_DESTINATION(destination)    ::krakoa::Logger::GetCoreLogger().ChangeOutputDirectory(destination);
#define KRK_CHANGE_LOGS_FILENAME(name)              ::krakoa::Logger::GetCoreLogger().ChangeFilename(name);
#define KRK_DBUG(msg)                               ::krakoa::Logger::GetCoreLogger().AddEntry(msg, ::klib::kLogs::LLevel::DBUG);
#define KRK_NORM(msg)                               ::krakoa::Logger::GetCoreLogger().AddEntry(msg, ::klib::kLogs::LLevel::NORM);
#define KRK_INFO(msg)                               ::krakoa::Logger::GetCoreLogger().AddEntry(msg, ::klib::kLogs::LLevel::INFO);
#define KRK_WARN(msg)                               ::krakoa::Logger::GetCoreLogger().AddEntry(msg, ::klib::kLogs::LLevel::WARN);
#define KRK_ERRR(msg)                               ::krakoa::Logger::GetCoreLogger().AddEntry(msg, ::klib::kLogs::LLevel::ERRR, __FILE__, __LINE__);
#define KRK_FATAL(msg)                              ::krakoa::Logger::GetCoreLogger().OutputToFatalFile(msg, __FILE__, __LINE__);
#define KRK_BANNER(banner, category)                ::krakoa::Logger::GetCoreLogger().AddEntryBanner(banner, category);
#define KRK_SUSPEND()                               ::krakoa::Logger::GetCoreLogger().SuspendFileLogging();
#define KRK_UNSUSPEND()                             ::krakoa::Logger::GetCoreLogger().UnsuspendFileLogging();
#define KRK_END()                                   ::krakoa::Logger::GetCoreLogger().FinalOutput();
#define KRK_GET_LAST()                              ::krakoa::Logger::GetCoreLogger().GetLastCachedEntry();
#define KRK_ERASE_PREV(numOfPrevEntries)            ::krakoa::Logger::GetCoreLogger().ErasePreviousCacheEntries(numOfPrevEntries);
#define KRK_CLEAR()                                 ::krakoa::Logger::GetCoreLogger().ClearCache();

// CLIENT SIDE Log Macros
#define INIT_LOGS()                             ::krakoa::Logger::ClientInit();
#define SET_MIN(minLvl)                         ::krakoa::Logger::GetClientLogger().SetMinimumLoggingLevel(minLvl);
#define TOGGLE_LOGGING()                        ::krakoa::Logger::GetClientLogger().ToggleLoggingEnabled();
#define DISABLE_FILE_OUTPUT()                   ::krakoa::Logger::GetClientLogger().SetCacheMode(true);
#define ENABLE_FILE_OUTPUT()                    ::krakoa::Logger::GetClientLogger().SetCacheMode(false);
#define CHANGE_LOGS_DESTINATION(destination)    ::krakoa::Logger::GetClientLogger().ChangeOutputDirectory(destination);
#define CHANGE_LOGS_FILENAME(name)              ::krakoa::Logger::GetClientLogger().ChangeFilename(name);
#define DBUG(msg)                               ::krakoa::Logger::GetClientLogger().AddEntry(msg, ::klib::kLogs::LLevel::DBUG);
#define NORM(msg)                               ::krakoa::Logger::GetClientLogger().AddEntry(msg, ::klib::kLogs::LLevel::NORM);
#define INFO(msg)                               ::krakoa::Logger::GetClientLogger().AddEntry(msg, ::klib::kLogs::LLevel::INFO);
#define WARN(msg)                               ::krakoa::Logger::GetClientLogger().AddEntry(msg, ::klib::kLogs::LLevel::WARN);
#define ERRR(msg)                               ::krakoa::Logger::GetClientLogger().AddEntry(msg, ::klib::kLogs::LLevel::ERRR, __FILE__, __LINE__);
#define FATAL(msg)                              ::krakoa::Logger::GetClientLogger().OutputToFatalFile(msg, __FILE__, __LINE__);
#define BANNER(banner, category)                ::krakoa::Logger::GetClientLogger().AddEntryBanner(banner, category);
#define SUSPEND()                               ::krakoa::Logger::GetClientLogger().SuspendFileLogging();
#define UNSUSPEND()                             ::krakoa::Logger::GetClientLogger().UnsuspendFileLogging();
#define END()                                   ::krakoa::Logger::GetClientLogger().FinalOutput();
#define GET_LAST()                              ::krakoa::Logger::GetClientLogger().GetLastCachedEntry();
#define ERASE_PREV(numOfPrevEntries)            ::krakoa::Logger::GetClientLogger().ErasePreviousCacheEntries(numOfPrevEntries);
#define CLEAR()                                 ::krakoa::Logger::GetClientLogger().ClearCache();
