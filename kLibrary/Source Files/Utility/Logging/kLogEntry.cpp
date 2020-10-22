#include <pch.hpp>
#include "kLogEntry.hpp"


namespace klib::kLogs
{
		LogEntry::LogEntry(const char* message, const char* file,
		                   const std::int64_t line, CalendarInfoSource calendarInfoSource)
			: time (calendarInfoSource)
			, date(calendarInfoSource)
			, msg(message)
			, file(file)
			, line(line)
		{}

		LogEntry::LogEntry(const std::string& message, const char* file, const std::int64_t line,
			CalendarInfoSource calendarInfoSource)
			: time(calendarInfoSource)
			, date(calendarInfoSource)
			, msg(message)
			, file(file)
			, line(line)
		{}
}
