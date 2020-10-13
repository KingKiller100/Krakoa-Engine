#pragma once

#include "../Calendar/kDateTime.hpp"
#include <string>
#include <cstdint>

namespace klib
{
	using namespace kCalendar;
	
	namespace kLogs
	{
		enum class LogLevel : unsigned short
		{
			BANR, // Log Banner
			DBUG, // Debug
			NORM, // Normal
			INFO, // Informative
			WARN, // Warning
			ERRR, // Error
			FATL  // Fatal
		};
		
		struct LogEntry
		{
		public:
			LogEntry(const std::string_view& message, const LogLevel level, const std::string_view& file, 
				const std::uint32_t line, CalendarInfoSource calendarInfoSource = CalendarInfoSource::LOCAL);

			Date date;
			Time time;
			LogLevel lvl;
			std::string msg;
			std::string file;
			std::uint32_t line;
		};
	}
}