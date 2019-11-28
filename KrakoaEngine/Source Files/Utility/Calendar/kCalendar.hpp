#pragma once

#include <string>

struct _SYSTEMTIME;

namespace util
{
	namespace kCalendar	
	{
		enum class TimeComponent
		{
			hour,
			min,
			sec,
			millisec
		};

		const _SYSTEMTIME& GetLocalDateAndTime();
		const _SYSTEMTIME& GetSystemDateAndTime();		
		unsigned short GetComponentOfTime(const TimeComponent timeComponent);

		// ASCII
		std::string GetTimeText();
		std::string GetDateInTextFormat();
		std::string_view GetMonth(const unsigned short month);
		std::string_view GetDayOfTheWeek(const unsigned short day);
		std::string GetDateInNumericalFormat(const bool slash = true);

		// Wide Multi-Byte Chars
		std::wstring wGetTimeText();
		std::wstring wGetDateInTextFormat();
		std::wstring_view wGetMonth(const unsigned short month);
		std::wstring_view wGetDayOfTheWeek(const unsigned short day);
		std::wstring wGetDateInNumericalFormat(const bool slash = true);
	}
}
