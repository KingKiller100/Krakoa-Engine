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
				const std::int32_t line = __LINE__, CalendarInfoSource calendarInfoSource = CalendarInfoSource::LOCAL);
			LogEntry(const std::string& message, const std::string_view& file = __FILE__,
				const std::int32_t line = __LINE__, CalendarInfoSource calendarInfoSource = CalendarInfoSource::LOCAL);

			const Time time;
			const Date date;
			const std::string msg;
			const std::string file;
			const std::int32_t line;
		};
	}

#ifdef KLIB_SHORT_NAMESPACE
		using namespace kLogs;
#endif
}
