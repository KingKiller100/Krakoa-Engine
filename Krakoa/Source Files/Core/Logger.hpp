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
		static void Init();
		inline static Logging& GetCoreLogger()            { return *coreLogger; }
		inline static Logging& GetClientLogger()          { return *clientLogger; }

	private:
		static std::unique_ptr<Logging> coreLogger;
		static std::unique_ptr<Logging> clientLogger;
	};
}

// Initializer
#define INIT_LOGS()                                 ::krakoa::Logger::Init();

// ENGINE SIDE Log Macros
#define KRK_SET_MIN(minLvl)                         ::krakoa::Logger::GetCoreLogger().SetMinimumLoggingLevel(minLvl);
#define KRK_TOGGLE_LOGGING()                        ::krakoa::Logger::GetCoreLogger().ToggleLoggingEnabled();
#define KRK_CHANGE_LOGS_DESTINATION(destination)    ::krakoa::Logger::GetCoreLogger().ChangeOutputDirectory(destination);
#define KRK_CHANGE_LOGS_FILENAME(name)              ::krakoa::Logger::GetCoreLogger().ChangeFilename(name);
#define KRK_DBUG(msg)                               ::krakoa::Logger::GetCoreLogger().AddEntry(msg, ::klib::kLogs::LLevel::DBUG);
#define KRK_NORM(msg)                               ::krakoa::Logger::GetCoreLogger().AddEntry(msg, ::klib::kLogs::LLevel::NORM);
#define KRK_INFO(msg)                               ::krakoa::Logger::GetCoreLogger().AddEntry(msg, ::klib::kLogs::LLevel::INFO);
#define KRK_WARN(msg)                               ::krakoa::Logger::GetCoreLogger().AddEntry(msg, ::klib::kLogs::LLevel::WARN);
#define KRK_ERRR(msg)                               ::krakoa::Logger::GetCoreLogger().AddEntry(msg, ::klib::kLogs::LLevel::ERRR, __FILE__, __LINE__);
#define KRK_FATAL(msg)                              ::krakoa::Logger::GetCoreLogger().OutputToFatalFile(msg, __FILE__, __LINE__);
#define KRK_BANNER(banner, category)                ::krakoa::Logger::GetCoreLogger().AddEntryBanner(banner, category);
#define KRK_FLUSH()                                 ::krakoa::Logger::GetCoreLogger().Output();
#define KRK_END()                                   ::krakoa::Logger::GetCoreLogger().FinalOutput();
#define KRK_GET_LAST()                              ::krakoa::Logger::GetCoreLogger().GetLastLoggedEntry();
#define KRK_ERASE_PREV(numOfPrevEntries)            ::krakoa::Logger::GetCoreLogger().ErasePreviousEntries(numOfPrevEntries);
#define KRK_CLEAR()                                 ::krakoa::Logger::GetCoreLogger().Clear();

// CLIENT SIDE Log Macros
#define SET_MIN(minLvl)                         ::krakoa::Logger::GetClientLogger().SetMinimumLoggingLevel(minLvl);
#define TOGGLE_LOGGING()                        ::krakoa::Logger::GetClientLogger().ToggleLoggingEnabled();
#define CHANGE_LOGS_DESTINATION(destination)    ::krakoa::Logger::GetClientLogger().ChangeOutputDirectory(destination);
#define CHANGE_LOGS_FILENAME(name)              ::krakoa::Logger::GetClientLogger().ChangeFilename(name);
#define DBUG(msg)                               ::krakoa::Logger::GetClientLogger().AddEntry(msg, ::klib::kLogs::LLevel::DBUG);
#define NORM(msg)                               ::krakoa::Logger::GetClientLogger().AddEntry(msg, ::klib::kLogs::LLevel::NORM);
#define INFO(msg)                               ::krakoa::Logger::GetClientLogger().AddEntry(msg, ::klib::kLogs::LLevel::INFO);
#define WARN(msg)                               ::krakoa::Logger::GetClientLogger().AddEntry(msg, ::klib::kLogs::LLevel::WARN);
#define ERRR(msg)                               ::krakoa::Logger::GetClientLogger().AddEntry(msg, ::klib::kLogs::LLevel::ERRR, __FILE__, __LINE__);
#define FATAL(msg)                              ::krakoa::Logger::GetClientLogger().OutputToFatalFile(msg, __FILE__, __LINE__);
#define BANNER(banner, category)                ::krakoa::Logger::GetClientLogger().AddEntryBanner(banner, category);
#define FLUSH()                                 ::krakoa::Logger::GetClientLogger().Output();
#define END()                                   ::krakoa::Logger::GetClientLogger().FinalOutput();
#define GET_LAST()                              ::krakoa::Logger::GetClientLogger().GetLastLoggedEntry();
#define ERASE_PREV(numOfPrevEntries)            ::krakoa::Logger::GetClientLogger().ErasePreviousEntries(numOfPrevEntries);
#define CLEAR()                                 ::krakoa::Logger::GetClientLogger().Clear();
