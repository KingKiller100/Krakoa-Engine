#pragma once

#include "../Debug Helper/Exceptions/CalenderExceptions.hpp"

#include <array>
#include <string>

struct _SYSTEMTIME;

namespace klib
{
	namespace kCalendar	
	{
		enum class TimeComponent : uint8_t
		{
			hours,
			mins,
			secs,
			millis
		};

		_SYSTEMTIME& GetLocalDateAndTime() noexcept;
		_SYSTEMTIME& GetSystemDateAndTime() noexcept;
		unsigned short GetComponentOfTime(const TimeComponent timeComponent);

		// ASCII
		std::string GetTimeText() noexcept;
		std::string GetDateInTextFormat(const bool fullDayName = true) noexcept;
		constexpr std::string_view GetMonth(const size_t month)
		{
			constexpr std::array<const char*, 12> kCalendar_MonthsArray = {
				"January", "February", "March",
				"April", "May", "June",
				"July", "August", "September",
				"October", "November", "December"
			};

			if (month < kCalendar_MonthsArray.size())
				return kCalendar_MonthsArray[month];

			throw kDebug::InvalidMonthError("Value entered does not index to a month of the year");
		}
		
		constexpr std::string_view GetDayOfTheWeek(const size_t day)
		{
			constexpr std::array<const char*, 7> kCalendar_DaysOfTheWeek = {
				"Sunday", "Monday", "Tuesday", "Wednesday",
			"Thursday", "Friday", "Saturday" };

			if (day < kCalendar_DaysOfTheWeek.size())
				return kCalendar_DaysOfTheWeek[day];

			throw kDebug::InvalidDayError("Value entered does not index to a day of the week");
		}
		
		std::string GetDateInNumericalFormat(const bool slash = true) noexcept;
		std::string CreateTime(unsigned short hour, unsigned short minute, unsigned short second) noexcept;
		std::string GetLocalStartTimeStr() noexcept;
		std::string GetSystemStartTimeStr() noexcept;
		std::string GetLocalStartDateStr(const bool slash = true) noexcept;
		//std::string GetSystemStartDateStr(const bool slash = true) noexcept;


		// Wide Multi-Byte Chars
		std::wstring wGetTimeText() noexcept;
		std::wstring wGetDateInTextFormat(const bool fullDayname = true) noexcept;
		constexpr std::wstring_view wGetMonth(const unsigned short month)
		{
			constexpr  std::array<const wchar_t*, 12> kCalendar_MonthsArray = {
				L"January", L"February", L"March",
				L"April", L"May", L"June",
				L"July", L"August", L"September",
				L"October", L"November", L"December"
			};

			if (month < kCalendar_MonthsArray.size())
				return kCalendar_MonthsArray[month];

			throw kDebug::InvalidMonthError("Value entered does not index to a month of the year");
		}
		constexpr std::wstring_view wGetDayOfTheWeek(const unsigned short day)
		{
			constexpr  std::array<const wchar_t*, 7> kCalendar_DaysOfTheWeek =
			{ L"Sunday", L"Monday", L"Tuesday", L"Wednesday",
			L"Thursday", L"Friday", L"Saturday" };

			if (day < kCalendar_DaysOfTheWeek.size())
				return kCalendar_DaysOfTheWeek[day];

			throw kDebug::InvalidDayError("Value entered does not index to a day of the week");
		}
		std::wstring wGetDateInNumericalFormat(const bool slash = true) noexcept;
		std::wstring wCreateTime(unsigned short hour, unsigned short minute, unsigned short second) noexcept;
		std::wstring wGetLocalStartTimeStr() noexcept;
		std::wstring wGetSystemStartTimeStr() noexcept;
		std::wstring wGetLocalStartDateStr(const bool slash = true) noexcept;
		//std::wstring wGetSystemStartDateStr(const bool slash = true) noexcept;
	}
}
