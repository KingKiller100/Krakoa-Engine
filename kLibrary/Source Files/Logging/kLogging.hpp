#pragma once

#include <Logging/kLogging_Class.hpp>

#include <memory>

namespace util::kLogs
{
	inline auto& GetMainLogger()
	{
		static auto mainlogger =
			std::unique_ptr<Logging>(new Logging());

		return mainlogger;
	}
}

#define INIT_LOGS()								::util::kLogs::GetMainLogger()->InitializeLogging();
#define CHANGE_LOGS_DESTINATION(destination)	::util::kLogs::GetMainLogger()->ChangeOutputDirectory(destination);
#define CHANGE_LOGS_FILENAME(name)				::util::kLogs::GetMainLogger()->ChangeFilename(name);
#define LOG_NORM(msg)							::util::kLogs::GetMainLogger()->AddEntry(msg, ::util::kLogs::LogLevel::NORM, __FILE__, __LINE__);
#define LOG_INFO(msg)							::util::kLogs::GetMainLogger()->AddEntry(msg, ::util::kLogs::LogLevel::INFO, __FILE__, __LINE__);
#define LOG_WARN(msg)							::util::kLogs::GetMainLogger()->AddEntry(msg, ::util::kLogs::LogLevel::WARN, __FILE__, __LINE__);
#define LOG_ERRR(msg)							::util::kLogs::GetMainLogger()->AddEntry(msg, ::util::kLogs::LogLevel::ERRR, __FILE__, __LINE__);
#define FATAL(msg)								::util::kLogs::GetMainLogger()->OutputToFatalFile(msg, __FILE__, __LINE__);
#define LOG_BANNER(banner, category)			::util::kLogs::GetMainLogger()->AddEntryBanner(banner, category);
#define FLUSH_LOGS()							::util::kLogs::GetMainLogger()->AppendLogFile();
#define END_LOGGING()							::util::kLogs::GetMainLogger()->FinalOutput();
#define GET_LAST_LOG_ENTRY()					::util::kLogs::GetMainLogger()->GetLastLoggedEntry();
#define ERASE_LOG_ENTRIES(amount)				::util::kLogs::GetMainLogger()->ErasePreviousEntries(amount);
#define CLEAR_LOGS()							::util::kLogs::GetMainLogger()->Clear();


