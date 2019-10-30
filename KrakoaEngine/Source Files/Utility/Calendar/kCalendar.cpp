#include "kCalendar.h"

#include "../Format To String/kFormatToString.h"

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
		switch (month)
		{
		case 1: return "January";
		case 2: return "February";
		case 3: return "March";
		case 4: return "April";
		case 5: return "May";
		case 6: return "June";
		case 7: return "July";
		case 8: return "August";
		case 9: return "September";
		case 10: return "October";
		case 11: return "November";
		case 12: return "December";
		default: return "Not a month";
		}
	}

	const char* GetDayOfTheWeek(const unsigned short day)
	{
		switch (day)
		{
		case 1: return "Monday";
		case 2: return "Tuesday";
		case 3: return "Wednesday";
		case 4: return "Thursday";
		case 5: return "Friday";
		case 6: return "Saturday";
		case 7: return "Sunday";
		default: return "Not a day";
		}
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
