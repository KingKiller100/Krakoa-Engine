#pragma once

#include "kDateTime.hpp"
#include <string>

namespace klib
{
	namespace kCalendar
	{
		unsigned short GetComponentOfTime(const TimeComponent timeComponent);

		// ASCII
		std::string GetTimeText(CalendarInfoSource calendarInfo = CalendarInfoSource::LOCAL) noexcept;
		std::string GetDateInTextFormat(const DateTextLength format) noexcept;


		std::string GetDateInNumericalFormat(const DateNumericalSeparator separator) noexcept;
		std::string CreateTime(uint8_t hours, uint8_t minutes) noexcept;
		std::string CreateTime(uint8_t hours, uint8_t minutes, uint8_t seconds) noexcept;
		std::string CreateTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds) noexcept;
		std::string GetLocalStartTimeStr() noexcept;
		std::string GetSystemStartTimeStr() noexcept;
		std::string GetLocalStartDateStr(const DateNumericalSeparator separator) noexcept;
		std::string GetSystemStartDateStr(const DateNumericalSeparator separator) noexcept;
	}

#ifdef KLIB_SHORTHAND_NAMESPACE
	using namespace kCalendar;
#endif
}
