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

		const SYSTEMTIME GetSystemDateAndTime();		
		unsigned short GetComponentOfTime(const TimeComponent timeComponent);

		// ASCII
		std::string GetTimeText();
		std::string GetDateInTextFormat();
		const char* GetMonth(const unsigned short month);
		const char* GetDayOfTheWeek(const unsigned short day);
		std::string GetDateInNumericalFormat(const bool slash = true);

		//UNICODE
		std::wstring wGetTimeText();
		std::wstring wGetDateInTextFormat();
		const wchar_t* wGetMonth(const unsigned short month);
		const wchar_t* wGetDayOfTheWeek(const unsigned short day);
		std::wstring wGetDateInNumericalFormat(const bool slash = true);
	}
}
