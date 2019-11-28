#include <Precompile.h>
#include <Utility/Calendar/kCalendar.hpp>

#include <Utility/Format/kFormatToString.hpp>

#include <array>
#include <Windows.h>

namespace util::kCalendar
{
	using namespace kFormat;

	const _SYSTEMTIME& GetLocalDateAndTime()
	{
		static _SYSTEMTIME kCalendar_Local_DateTime;
		GetLocalTime(&kCalendar_Local_DateTime);
		return kCalendar_Local_DateTime;
	}

	const _SYSTEMTIME& GetSystemDateAndTime()
	{
		static _SYSTEMTIME kCalendar_System_DateTime;
		GetSystemTime(&kCalendar_System_DateTime);
		return kCalendar_System_DateTime;
	}

	unsigned short GetComponentOfTime(const TimeComponent timeComponent)
	{
		const auto now = GetLocalDateAndTime();

		switch (timeComponent)
		{
		case TimeComponent::hour:		return now.wHour;
		case TimeComponent::min:		return now.wMinute;
		case TimeComponent::sec:		return now.wSecond;
		case TimeComponent::millisec:	return now.wMilliseconds;
		default: return MAXWORD;
		}
	}

	// ASCII
	
	std::string GetTimeText()
	{
		const auto dateTime = GetLocalDateAndTime();
		return FormatToString("%02d:%02d:%02d:%03d", dateTime.wHour, dateTime.wMinute, dateTime.wSecond, dateTime.wMilliseconds);
	}

	std::string GetDateInTextFormat()
	{
		const auto dateTime = GetLocalDateAndTime();				
		return FormatToString("%s %d %s %04d", GetDayOfTheWeek(dateTime.wDayOfWeek), dateTime.wDay, GetMonth(dateTime.wMonth), dateTime.wYear);
	}

	std::string_view GetMonth(const unsigned short month)
	{
		static std::array<const char*, 12> kCalendar_MonthsArray = 
		{"January", "February", "March", "April", "May",
		"June", "July", "August", "September", "October",
			"November", "December"};

		if(month < kCalendar_MonthsArray.size())
			return kCalendar_MonthsArray[month - 1];

		return "Value entered does not index to a month of the year";
	}

	std::string_view GetDayOfTheWeek(const unsigned short day)
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
		const auto dateTime = GetLocalDateAndTime();
		const auto dateFormat = slash ? "%02d/%02d/%02d" : "%02d-%02d-%04d";
		return FormatToString(dateFormat, dateTime.wDay, dateTime.wMonth, dateTime.wYear);
	}

	// WIDE MULTI-BYTE CHAR
	
	std::wstring wGetTimeText()
	{
		const auto dateTime = GetLocalDateAndTime();
		return FormatToString(L"%02d:%02d:%02d:%03d", dateTime.wHour, dateTime.wMinute, dateTime.wSecond, dateTime.wMilliseconds);
	}

	std::wstring wGetDateInTextFormat()
	{
		const auto dateTime = GetLocalDateAndTime();
		return FormatToString(L"%s %d %s %04d", GetDayOfTheWeek(dateTime.wDayOfWeek), dateTime.wDay, GetMonth(dateTime.wMonth), dateTime.wYear);
	}

	std::wstring_view  wGetMonth(const unsigned short month)
	{
		static std::array<const wchar_t*, 12> kCalendar_MonthsArray =
		{ L"January", L"February", L"March", L"April", L"May",
		L"June", L"July", L"August", L"September", L"October",
			L"November", L"December" };

		if (month < kCalendar_MonthsArray.size())
			return kCalendar_MonthsArray[month - 1];

		return L"Value entered does not index to a month of the year";
	}

	std::wstring_view  wGetDayOfTheWeek(const unsigned short day)
	{
		static std::array<const wchar_t*, 7> kCalendar_DaysOfTheWeek =
		{ L"Sunday", L"Monday", L"Tuesday", L"Wednesday",
		L"Thursday", L"Friday", L"Saturday" };

		if (day < kCalendar_DaysOfTheWeek.size())
			return kCalendar_DaysOfTheWeek[day];

		return L"Value entered does not index to a day of the week";
	}

	std::wstring wGetDateInNumericalFormat(const bool slash)
	{
		const auto dateTime = GetLocalDateAndTime();
		const auto dateFormat = slash ? L"%02d/%02d/%02d" : L"%02d-%02d-%04d";
		return FormatToString(dateFormat, dateTime.wDay, dateTime.wMonth, dateTime.wYear);
	}
}
