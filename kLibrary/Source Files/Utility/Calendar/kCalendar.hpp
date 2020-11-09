#pragma once

#include "kTime.hpp"
#include <string>

#include "Date/kDate.hpp"

namespace klib
{
	namespace kCalendar
	{
		std::string GetLocalStartDateStr(const Date::DateNumericalSeparator separator) noexcept;
		std::string GetSystemStartDateStr(const Date::DateNumericalSeparator separator) noexcept;
		std::string GetDateInTextFormat(const Date::DateTextLength format, const CalendarInfoSourceType source = CalendarInfoSourceType::LOCAL) noexcept;
		std::string GetDateInNumericalFormat(const Date::DateNumericalSeparator separator, const CalendarInfoSourceType  source = CalendarInfoSourceType::LOCAL) noexcept;


		unsigned short GetComponentOfTime(const Time::TimeComponent timeComponent, const CalendarInfoSourceType source = CalendarInfoSourceType::LOCAL);
		std::string GetTimeText(CalendarInfoSourceType calendarInfo = CalendarInfoSourceType::LOCAL) noexcept;
		std::string CreateTime(uint8_t hours, uint8_t minutes) noexcept;
		std::string CreateTime(uint8_t hours, uint8_t minutes, uint8_t seconds) noexcept;
		std::string CreateTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds) noexcept;
		std::string GetLocalStartTimeStr() noexcept;
		std::string GetSystemStartTimeStr() noexcept;
	}

#ifdef KLIB_SHORT_NAMESPACE
	using namespace kCalendar;
#endif
}
