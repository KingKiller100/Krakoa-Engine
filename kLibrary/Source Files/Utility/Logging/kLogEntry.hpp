#pragma once

#include "../Calendar/kDateTime.hpp"
#include <string>
#include <cstdint>

#include "kLogLevel.hpp"

namespace klib
{
	namespace kLogs
	{		
		using namespace kCalendar;
	
		struct LogEntry
		{
			LogEntry(const std::string_view& message, const LogLevel lvl, const std::string_view& file, 
				const std::uint32_t line, CalendarInfoSource calendarInfoSource = CalendarInfoSource::LOCAL);

			const Time time;
			const Date date;
			const LogLevel lvl;
			std::string msg;
			const std::string file;
			const std::uint32_t line;
		};
	}

#ifdef KLIB_SHORTHAND_NAMESPACE
		using namespace kLogs;
#endif
}
