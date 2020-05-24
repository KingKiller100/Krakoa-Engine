#pragma once

#include "../Debug Helper/Exceptions/CalenderExceptions.hpp"

#include <array>
#include <string>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#	include <Windows.h>
#undef WIN32_LEAN_AND_MEAN

namespace klib
{
	namespace kCalendar
	{
		enum class TimeComponent : uint8_t
		{
			HOURS,
			MINS,
			SECS,
			MILLIS
		};

		enum class DateFormat : uint8_t
		{
			FULL,
			SHORT
		};

		enum class DateSeparator :uint8_t
		{
			SLASH,
			DASH,
		};

		_SYSTEMTIME& GetLocalDateAndTime() noexcept;
		_SYSTEMTIME& GetSystemDateAndTime() noexcept;
		unsigned short GetComponentOfTime(const TimeComponent timeComponent);

		// ASCII
		std::string GetTimeText(const _SYSTEMTIME dateTime = GetLocalDateAndTime()) noexcept;
		std::string GetDateInTextFormat(const DateFormat format = DateFormat::FULL, const _SYSTEMTIME& dateTime = GetLocalDateAndTime()) noexcept;
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
			"Thursday", "Friday", "Saturday"
			};

			if (day < kCalendar_DaysOfTheWeek.size())
				return kCalendar_DaysOfTheWeek[day];

			throw kDebug::InvalidDayError("Value entered does not index to a day of the week");
		}

		std::string GetDateInNumericalFormat(const DateSeparator separator = DateSeparator::SLASH, const _SYSTEMTIME& dateTime = GetLocalDateAndTime()) noexcept;
		std::string CreateTime(uint8_t hours, uint8_t minutes) noexcept;
		std::string CreateTime(uint8_t hours, uint8_t minutes, uint8_t seconds) noexcept;
		std::string CreateTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds) noexcept;
		std::string GetLocalStartTimeStr() noexcept;
		std::string GetSystemStartTimeStr() noexcept;
		std::string GetLocalStartDateStr(const DateSeparator separator = DateSeparator::SLASH) noexcept;
		std::string GetSystemStartDateStr(const DateSeparator separator = DateSeparator::SLASH) noexcept;


		// Wide Multi-Byte Chars
		std::wstring wGetTimeText(const _SYSTEMTIME dateTime = GetLocalDateAndTime()) noexcept;
		std::wstring wGetDateInTextFormat(const DateFormat format = DateFormat::FULL, const _SYSTEMTIME& dateTime = GetLocalDateAndTime()) noexcept;
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
			constexpr  std::array<const wchar_t*, 7> kCalendar_DaysOfTheWeek = {
				L"Sunday", L"Monday", L"Tuesday", L"Wednesday",
			L"Thursday", L"Friday", L"Saturday"
			};

			if (day < kCalendar_DaysOfTheWeek.size())
				return kCalendar_DaysOfTheWeek[day];

			throw kDebug::InvalidDayError("Value entered does not index to a day of the week");
		}
		std::wstring wGetDateInNumericalFormat(const DateSeparator separator = DateSeparator::SLASH, const _SYSTEMTIME& dateTime = GetLocalDateAndTime()) noexcept;
		std::wstring wCreateTime(uint8_t hours, uint8_t minutes) noexcept;
		std::wstring wCreateTime(uint8_t hours, uint8_t minutes, uint8_t seconds) noexcept;
		std::wstring wCreateTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds) noexcept;
		std::wstring wGetLocalStartTimeStr() noexcept;
		std::wstring wGetSystemStartTimeStr() noexcept;
		std::wstring wGetLocalStartDateStr(const DateSeparator separator = DateSeparator::SLASH) noexcept;
		std::wstring wGetSystemLocalStartDateStr(const DateSeparator separator = DateSeparator::SLASH) noexcept;
	}
}
