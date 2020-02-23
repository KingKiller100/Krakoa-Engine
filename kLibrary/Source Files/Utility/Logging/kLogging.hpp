#pragma once

#include <Utility/Logging/kLogging_Class.hpp>

#include <memory>

namespace klib::kLogs
{
	namespace
	{
		auto mainLogger = std::make_unique<Logging>();
	}
}

#define INIT_LOGS(initialLevel)					::klib::kLogs::mainLogger->InitializeLogging(initialLevel);
#define TOGGLE_LOGGING()						::klib::kLogs::mainLogger->ToggleLoggingEnabled();
#define CHANGE_LOGS_DESTINATION(destination)	::klib::kLogs::mainLogger->ChangeOutputDirectory(destination);
#define CHANGE_LOGS_FILENAME(name)				::klib::kLogs::mainLogger->ChangeFilename(name);
#define LOG_NORM(msg)							::klib::kLogs::mainLogger->AddEntry(msg, ::klib::kLogs::LLevel::NORM, __FILE__, __LINE__);
#define LOG_INFO(msg)							::klib::kLogs::mainLogger->AddEntry(msg, ::klib::kLogs::LLevel::INFO, __FILE__, __LINE__);
#define LOG_WARN(msg)							::klib::kLogs::mainLogger->AddEntry(msg, ::klib::kLogs::LLevel::WARN, __FILE__, __LINE__);
#define LOG_ERRR(msg)							::klib::kLogs::mainLogger->AddEntry(msg, ::klib::kLogs::LLevel::ERRR, __FILE__, __LINE__);
#define FATAL(msg)								::klib::kLogs::mainLogger->OutputToFatalFile(msg, __FILE__, __LINE__);
#define LOG_BANNER(banner, category)			::klib::kLogs::mainLogger->AddEntryBanner(banner, category);
#define FLUSH_LOGS()							::klib::kLogs::mainLogger->AppendLogFile();
#define END_LOGGING()							::klib::kLogs::mainLogger->FinalOutput();
#define GET_LAST_LOG_ENTRY()					::klib::kLogs::mainLogger->GetLastLoggedEntry();
#define ERASE_LOG_ENTRIES(amount)				::klib::kLogs::mainLogger->ErasePreviousEntries(amount);
#define CLEAR_LOGS()							::klib::kLogs::mainLogger->Clear();


