#include <pch.hpp>
#include "kLogEntry.hpp"


namespace klib
{
	namespace kLogs
	{
		LogEntry::LogEntry(const std::string_view& message, const std::string_view& file,
		                   const std::uint32_t line, CalendarInfoSource calendarInfoSource)
			: time (calendarInfoSource)
			, date(calendarInfoSource)
			, msg(message)
			, file(file)
			, line(line)
		{}
	}
}