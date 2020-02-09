#pragma once

#include <Utility/Logging/kLogging_Class.hpp>

#include <memory>

namespace klib::kLogs
{
	static std::shared_ptr<Logging> mainlogger;

	static void Initialize()
	{

		if (!(mainlogger))
			mainlogger = std::shared_ptr<Logging>(new Logging(), [](Logging* ptr) {
			if (ptr->GetLastLoggedEntry() != Logging::kLogs_Empty)
				ptr->FinalOutput();
				});
		mainlogger->InitializeLogging();
	}

	static auto& GetMainLogger()
	{
		return *mainlogger;
	}
}

#define INIT_LOGS()								::klib::kLogs::Initialize();
#define CHANGE_LOGS_DESTINATION(destination)	::klib::kLogs::GetMainLogger().ChangeOutputDirectory(destination);
#define CHANGE_LOGS_FILENAME(name)				::klib::kLogs::GetMainLogger().ChangeFilename(name);
#define LOG_NORM(msg)							::klib::kLogs::GetMainLogger().AddEntry(msg, ::klib::kLogs::LogLevel::NORM, __FILE__, __LINE__);
#define LOG_INFO(msg)							::klib::kLogs::GetMainLogger().AddEntry(msg, ::klib::kLogs::LogLevel::INFO, __FILE__, __LINE__);
#define LOG_WARN(msg)							::klib::kLogs::GetMainLogger().AddEntry(msg, ::klib::kLogs::LogLevel::WARN, __FILE__, __LINE__);
#define LOG_ERRR(msg)							::klib::kLogs::GetMainLogger().AddEntry(msg, ::klib::kLogs::LogLevel::ERRR, __FILE__, __LINE__);
#define FATAL(msg)								::klib::kLogs::GetMainLogger().OutputToFatalFile(msg, __FILE__, __LINE__);
#define LOG_BANNER(banner, category)			::klib::kLogs::GetMainLogger().AddEntryBanner(banner, category);
#define FLUSH_LOGS()							::klib::kLogs::GetMainLogger().AppendLogFile();
#define END_LOGGING()							::klib::kLogs::GetMainLogger().FinalOutput();
#define GET_LAST_LOG_ENTRY()					::klib::kLogs::GetMainLogger().GetLastLoggedEntry();
#define ERASE_LOG_ENTRIES(amount)				::klib::kLogs::GetMainLogger().ErasePreviousEntries(amount);
#define CLEAR_LOGS()							::klib::kLogs::GetMainLogger().Clear();


