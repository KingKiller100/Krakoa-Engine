#pragma once

#include <string>
#include <Windows.h>

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

		std::string GetTimeText();
		std::string GetDateInTextFormat();
		const SYSTEMTIME GetSystemDateAndTime();		
		const char* GetMonth(const unsigned short month);
		const char* GetDayOfTheWeek(const unsigned short day);
		std::string GetDateInNumericalFormat(const bool slash = true);
		unsigned short GetComponentOfTime(const TimeComponent timeComponent);
	}
}
