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

		const Date& LogEntry::GetDate() const
		{
			return date;
		}

		const Time& LogEntry::GetTime() const
		{
			return time;
		}

		LogLevel LogEntry::GetLevel() const
		{
			return lvl;
		}

		const std::string& LogEntry::GetMsg() const
		{
			return msg;
		}

		const std::string& LogEntry::GetFile() const
		{
			return file;
		}

		std::uint32_t LogEntry::GetLine() const
		{
			return line;
		}
	}
}