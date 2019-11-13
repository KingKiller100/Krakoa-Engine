#pragma once

#include "kLogging.h"
#include <memory>

namespace util::kLogs
{
	inline auto& GetLogger()
	{
		static auto kLogger_deletor = [](Logging* ptr)
		{
			ptr->Output();
			if (ptr)
				delete ptr;
		};

		static auto logger =
			std::unique_ptr<Logging, decltype(kLogger_deletor)>(new Logging(), kLogger_deletor);

		return logger;
	}

	inline auto& w_GetLogger()
	{
		static auto kLogger_deletor = [](Logging* ptr)
		{
			ptr->Output();
			if (ptr)
				delete ptr;
		};

		static auto logger =
			std::unique_ptr<Logging, decltype(kLogger_deletor)>(new Logging(), kLogger_deletor);

		return logger;
	}


#define LOG_ENTRY(msg, logLvl)	GetLogger()->AddEntry(msg, logLvl);
#define LOG_BANNER(banner, category) GetLogger()->AddEntryBanner(banner, category);
}


