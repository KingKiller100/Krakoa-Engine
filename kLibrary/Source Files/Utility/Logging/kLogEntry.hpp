#pragma once

#include "../Calendar/kDateTime.hpp"
#include <string>
#include <cstdint>


namespace klib
{
	namespace kLogs
	{		
		using namespace kCalendar;
	
		struct LogEntry
		{
			LogEntry(const char* message, const char* file = __FILE__, 
				const std::int64_t line = __LINE__, CalendarInfoSource calendarInfoSource = CalendarInfoSource::LOCAL);
			LogEntry(const std::string& message, const char* file = __FILE__, 
				const std::int64_t line = __LINE__, CalendarInfoSource calendarInfoSource = CalendarInfoSource::LOCAL);

			const Time time;
			const Date date;
			std::string msg;
			const std::string file;
			const std::int64_t line;
		};
	}

#ifdef KLIB_SHORTHAND_NAMESPACE
		using namespace kLogs;
#endif
}
