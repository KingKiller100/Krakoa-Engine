#pragma once

#include "../Calendar/kDateTime.hpp"
#include <string>
#include <cstdint>


namespace klib::kLogs
{
	using namespace kCalendar;

	struct LogMessage
	{
		LogMessage(const char* text, const char* file = __FILE__,
			const std::int32_t line = __LINE__, CalendarSourceType calendarInfoSource = CalendarSourceType::LOCAL);
		LogMessage(const std::string& text, const std::string_view& file = __FILE__,
			const std::int32_t line = __LINE__, CalendarSourceType calendarInfoSource = CalendarSourceType::LOCAL);
		LogMessage(const std::string& text, const LogMessage& other);
		
		const Time time;
		const Date date;
		const std::string text;
		const std::string file;
		const std::int32_t line;
	};
}
