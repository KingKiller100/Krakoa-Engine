#pragma once

#include "kLogging_Class.h"

#include <memory>

namespace util::kLogs
{
	inline auto& GetLogger()
	{
		static auto kLogger_deletor = [](Logging* ptr)
		{
			ptr->FinalOutput();
			
			delete ptr;
			ptr = nullptr;
		};

		static auto logger =
			std::unique_ptr<Logging, decltype(kLogger_deletor)>(new Logging(), kLogger_deletor);

		return logger;
	}

	/*inline auto& w_GetLogger()
	{
		static auto kLogger_deletor = [](Logging* ptr)
		{
			ptr->FinalOutput();
			if (ptr)
				delete ptr;
		};

		static auto logger =
			std::unique_ptr<Logging, decltype(kLogger_deletor)>(new Logging(), kLogger_deletor);

		return logger;
	}*/

}

#define INIT_LOGS								util::kLogs::GetLogger()->InitializeLogging();
#define CHANGE_LOGS_DESTINATION(destination)	util::kLogs::GetLogger()->ChangeOutputDirectory(destination);
#define CHANGE_LOGS_FILENAME(name)				util::kLogs::GetLogger()->ChangeFilename(name);
#define LOG_ENTRY(msg, logLvl)					util::kLogs::GetLogger()->AddEntry(msg, logLvl, __FILE__, __LINE__);
#define LOG_BANNER(banner, category)			util::kLogs::GetLogger()->AddEntryBanner(banner, category);
#define FLUSH_LOGS								util::kLogs::GetLogger()->AppendLogFile();
#define END_LOGGING								util::kLogs::GetLogger()->FinalOutput();
#define FATAL(msg)								util::kLogs::GetLogger()->OutputToFatalFile(msg, __FILE__, __LINE__);
#define GET_LAST_LOG_ENTRY						util::kLogs::GetLogger()->GetLastLoggedEntry();
#define ERASE_LOG_ENTRIES(amount)				util::kLogs::GetLogger()->ErasePreviousEntries(amount);
#define CLEAR_LOGS								util::kLogs::GetLogger()->Clear();


