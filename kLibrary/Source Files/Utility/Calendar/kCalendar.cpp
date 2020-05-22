#include "pch.hpp"
#include "kCalendar.hpp"

#include "../Format/kFormatToString.hpp"
#include "../String/kStringManipulation.hpp"

#include <array>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#	include <Windows.h>

#undef WIN32_LEAN_AND_MEAN

#ifdef  max
#	undef max
#endif

namespace klib::kCalendar
{
	using namespace kFormat;

	static const auto localStartTime = GetLocalDateAndTime();
	static const auto systemStartTime = GetSystemDateAndTime();

	_SYSTEMTIME& GetLocalDateAndTime()  noexcept
	{
		static _SYSTEMTIME kCalendar_Local_DateTime;
		GetLocalTime(&kCalendar_Local_DateTime);
		return kCalendar_Local_DateTime;
	}

	_SYSTEMTIME& GetSystemDateAndTime()  noexcept
	{
		static _SYSTEMTIME kCalendar_System_DateTime;
		GetSystemTime(&kCalendar_System_DateTime);
		return kCalendar_System_DateTime;
	}

	unsigned short GetComponentOfTime(const TimeComponent timeComponent)
	{
		const auto now = GetLocalDateAndTime();

		switch (timeComponent) {
		case TimeComponent::hours:		return now.wHour;
		case TimeComponent::mins:		return now.wMinute;
		case TimeComponent::secs:		return now.wSecond;
		case TimeComponent::millis:		return now.wMilliseconds;
		default: throw kDebug::CalendarError();
		}
	}

	// ASCII

	std::string GetTimeText()  noexcept
	{
		const auto dateTime = GetLocalDateAndTime();
		const auto dateStr = ToString("{0:2}:{1:2}:{2:2}:{3:3}", 
			dateTime.wHour, 
			dateTime.wMinute, 
			dateTime.wSecond, 
			dateTime.wMilliseconds);
		return dateStr;
	}

	std::string GetDateInTextFormat(const bool fullDayName)  noexcept
	{
		const auto dateTime = GetLocalDateAndTime();
		std::string day = GetDayOfTheWeek(dateTime.wDayOfWeek).data();
		day = fullDayName ? day : day.substr(0, 3);
		const auto dateStr = ToString("%s %d %s %04d", 
			day.data(), 
			dateTime.wDay, 
			GetMonth(dateTime.wMonth - 1).data(), 
			dateTime.wYear);
		return dateStr;
	}

	std::string GetDateInNumericalFormat(const bool slash)  noexcept
	{
		const auto dateTime = GetLocalDateAndTime();
		const auto dateFormat = slash ? "%02d/%02d/%02d" : "%02d-%02d-%04d";
		return ToString(dateFormat, dateTime.wDay, dateTime.wMonth, dateTime.wYear);
	}

	std::string CreateTime(unsigned short hour, unsigned short minute, unsigned short second)  noexcept
	{
		const auto time = ToString("%02d:%02d:%02d", hour, minute, second);
		return time;
	}

	std::string GetLocalStartTimeStr() noexcept
	{
		static const auto localTimeStr =
			ToString("%02d:%02d:%02d:%03d",
				localStartTime.wHour,
				localStartTime.wMinute,
				localStartTime.wSecond,
				localStartTime.wMilliseconds);
		return localTimeStr;
	}

	std::string GetSystemStartTimeStr() noexcept
	{
		static const auto systemTimeStr =
			ToString("%02d:%02d:%02d:%03d",
				systemStartTime.wHour,
				systemStartTime.wMinute,
				systemStartTime.wSecond,
				systemStartTime.wMilliseconds);
		return systemTimeStr;
	}

	std::string GetLocalStartDateStr(const bool slash) noexcept
	{
		static std::string localDate;
		if (localDate.empty())
		{
			localDate = GetDateInNumericalFormat(slash);
			return localDate;
		}

		if (slash)
		{
			if (localDate.find('/') == std::string::npos)
				localDate = kString::Replace<char>(localDate, '-', '/');
		}
		else
		{
			if (localDate.find('-') == std::string::npos)
				localDate = kString::Replace<char>(localDate, '/', '-');
		}

		return localDate;
	}

	// WIDE MULTI-BYTE CHAR

	std::wstring wGetTimeText()  noexcept
	{
		const auto dateTime = GetLocalDateAndTime();
		return ToString(L"%02d:%02d:%02d:%03d", dateTime.wHour, dateTime.wMinute, dateTime.wSecond, dateTime.wMilliseconds);
	}

	std::wstring wGetDateInTextFormat(const bool fullDayname)  noexcept
	{
		const auto dateTime = GetLocalDateAndTime();
		std::wstring day = wGetDayOfTheWeek(dateTime.wDayOfWeek).data();
		day = fullDayname ? day : day.substr(0, 3);
		return ToString(L"%s %d %s %04d", day.data(), dateTime.wDay, GetMonth(dateTime.wMonth - 1).data(), dateTime.wYear);
	}

	std::wstring wGetDateInNumericalFormat(const bool slash) noexcept
	{
		const auto dateTime = GetLocalDateAndTime();
		const auto* const dateFormat = slash ? L"%02d/%02d/%02d" : L"%02d-%02d-%04d";
		return ToString(dateFormat, dateTime.wDay, dateTime.wMonth, dateTime.wYear);
	}

	std::wstring wCreateTime(unsigned short hour, unsigned short minute, unsigned short second) noexcept
	{
		const auto time = ToString(L"%02d:%02d:%02d", hour, minute, second);
		return time;
	}

	std::wstring wGetLocalStartTimeStr() noexcept
	{
		static const auto localTimeStr =
			ToString(L"%02d:%02d:%02d:%03d",
				localStartTime.wHour,
				localStartTime.wMinute,
				localStartTime.wSecond,
				localStartTime.wMilliseconds);
		return localTimeStr;
	}

	std::wstring wGetSystemStartTimeStr() noexcept
	{
		static const auto systemTimeStr =
			ToString(L"%02d:%02d:%02d:%03d",
				systemStartTime.wHour,
				systemStartTime.wMinute,
				systemStartTime.wSecond,
				systemStartTime.wMilliseconds);
		return systemTimeStr;
	}

	std::wstring wGetLocalStartDateStr(const bool slash) noexcept
	{
		static std::wstring localDate;
		if (localDate.empty())
		{
			localDate = wGetDateInNumericalFormat(slash);
			return localDate;
		}

		if (slash)
		{
			if (localDate.find('/') == std::wstring::npos)
				localDate = kString::Replace<wchar_t>(localDate, L'-', L'/');
		}
		else
		{
			if (localDate.find('-') == std::wstring::npos)
				localDate = kString::Replace<wchar_t>(localDate, L'/', L'-');
		}

		return localDate;
	}
}


