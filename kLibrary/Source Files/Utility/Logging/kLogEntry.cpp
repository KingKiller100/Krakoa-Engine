#include <pch.hpp>
#include "kLogEntry.hpp"


namespace klib
{
	using namespace kCalendar;

	namespace kLogs
	{
		EntryBase::EntryBase(const std::string_view& message, CalendarInfoSource calendarInfoSource)
			: date(calendarInfoSource)
			, time(calendarInfoSource)
			, msg(message)
		{}

		BannerEntry::BannerEntry(const std::string& msg, CalendarInfoSource calendarInfoSource, const std::string& type,
			const std::string& frontPadding, const std::string& backPadding, const std::uint16_t paddingCount)
			: EntryBase(msg, calendarInfoSource)
			, type(type)
			, frontPadding(frontPadding)
			, backPadding(backPadding)
			, paddingCount(paddingCount)
		{}

		LogEntry::LogEntry(const std::string_view& message, const LogLevel level, const std::string_view& file,
		                   const std::uint32_t line, CalendarInfoSource calendarInfoSource)
			: EntryBase(msg, calendarInfoSource)
			, lvl(level)
			, file(file)
			, line(line)
		{}

	}
}