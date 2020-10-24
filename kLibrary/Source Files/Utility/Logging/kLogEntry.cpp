#include <pch.hpp>
#include "kLogEntry.hpp"


namespace klib::kLogs
{
		LogEntry::LogEntry(const char* message, const char* file,
		                   const std::int32_t line, CalendarInfoSource calendarInfoSource)
			: time (calendarInfoSource)
			, date(calendarInfoSource)
			, msg(message)
			, file(file)
			, line(line)
		{}

		LogEntry::LogEntry(const std::string& message, const std::string_view& file, const std::int32_t line,
			CalendarInfoSource calendarInfoSource)
			: time(calendarInfoSource)
			, date(calendarInfoSource)
			, msg(message)
			, file(file)
			, line(line)
		{}
}
