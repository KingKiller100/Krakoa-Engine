#pragma once

#include "kLogEntry.h"
#include "kLogging.h"

#include <memory>

namespace util
{
	namespace kLogs
	{
		static std::unique_ptr<Logging> logger = std::make_unique<Logging>();

#define LOG_ENTRY(msg, logLvl)\
	{\
		const auto logEntry = kLogs::TemplateLogEntry<decltype(msg)>(msg, timeStr, date, __FILE__, __LINE__);\
		logger->AddEntry(logEntry, logLvl);\
	}\
		
	}
}

