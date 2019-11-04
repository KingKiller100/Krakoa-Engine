#include "kCalendar.h"

#include "../Format To String/kFormatToString.h"

#include <array>

namespace util::kCalendar
{
	using namespace kFormatToString;

	const SYSTEMTIME GetSystemDateAndTime()
	{
		static SYSTEMTIME kCalendarDateTime;
		GetLocalTime(&kCalendarDateTime);
		return kCalendarDateTime;
	}

	std::string GetTimeText()
	{
		const auto dateTime = GetSystemDateAndTime();
		return FormatToString("%02d:%02d:%02d:%03d", dateTime.wHour, dateTime.wMinute, dateTime.wSecond, dateTime.wMilliseconds);
	}

	std::string GetDateInTextFormat()
	{
		const auto dateTime = GetSystemDateAndTime();				
		return FormatToString("%s %d %s %04d", GetDayOfTheWeek(dateTime.wDayOfWeek), dateTime.wDay, GetMonth(dateTime.wMonth), dateTime.wYear);
	}

	const char* GetMonth(const unsigned short month)
	{
		static std::array<const char*, 12> kCalendar_MonthsArray = 
		{"January", "February", "March", "April", "May",
		"June", "July", "August", "September", "October",
			"November", "December"};

		if(month < kCalendar_MonthsArray.size())
			return kCalendar_MonthsArray[month - 1];

		return "Value entered does not index to a month of the year";
	}

	const char* GetDayOfTheWeek(const unsigned short day)
	{
		static std::array<const char*, 7> kCalendar_DaysOfTheWeek =
		{ "Sunday", "Monday", "Tuesday", "Wednesday",
		"Thursday", "Friday", "Saturday" };

		if (day < kCalendar_DaysOfTheWeek.size())
			return kCalendar_DaysOfTheWeek[day];

		return "Value entered does not index to a day of the week";
	}

	std::string GetDateInNumericalFormat(const bool slash)
	{
		const auto dateTime = GetSystemDateAndTime();
		const auto dateFormat = slash ? "%02d/%02d/%02d" : "%02d-%02d-%04d";
		return FormatToString(dateFormat, dateTime.wDay, dateTime.wMonth, dateTime.wYear);
	}

	unsigned short GetComponentOfTime(const TimeComponent timeComponent)
	{
		const auto now = GetSystemDateAndTime();

		switch (timeComponent)
		{
		case TimeComponent::hour: return now.wHour;
		case TimeComponent::min: return now.wMinute;
		case TimeComponent::sec: return now.wSecond;
		case TimeComponent::millisec: return now.wMilliseconds;
		default: return MAXWORD;
		}
	}
}
