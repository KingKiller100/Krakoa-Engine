#pragma once

#include "../Calendar/kDateTime.hpp"
#include <string>
#include <cstdint>

namespace klib
{
	using namespace kCalendar;
	
	namespace kLogs
	{
		struct EntryBase
		{
			EntryBase(const std::string_view& message, CalendarInfoSource calendarInfoSource);

			const Date date;
			const Time time;
			const std::string msg;
		};

		struct BannerEntry : public EntryBase
		{
			BannerEntry(const std::string& msg, CalendarInfoSource calendarInfoSource, const std::string& type
				, const std::string& frontPadding, const std::string& backPadding, const std::uint16_t paddingCount);

			const std::string type;
			const std::string frontPadding;
			const std::string backPadding;
			const std::uint16_t paddingCount;
		};
		
		struct LogEntry : public EntryBase
		{
		public:

		public:
			LogEntry(const std::string_view& message, const LogLevel level, const std::string_view& file, 
				const std::uint32_t line, CalendarInfoSource calendarInfoSource = CalendarInfoSource::LOCAL);

			const LogLevel lvl;
			const std::string file;
			const std::uint32_t line;
		};
	}
}