#include <pch.hpp>
#include "kLogEntry.hpp"


namespace klib
{
	using namespace kCalendar;

	namespace kLogs
	{
		LogEntry::LogEntry(const std::string_view& message, const LogLevel level, const std::string_view& file,
			const std::uint32_t line, CalendarInfoSource calendarInfoSource)
			: date(calendarInfoSource)
			, time(calendarInfoSource)
			, lvl(level)
			, file(file)
			, line(line)
			, msg(message)
		{
		}
	}
}