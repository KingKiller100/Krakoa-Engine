#pragma once

#include <string>

struct _SYSTEMTIME;

namespace klib
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

		_SYSTEMTIME& GetLocalDateAndTime() noexcept;
		_SYSTEMTIME& GetSystemDateAndTime() noexcept;
		unsigned short GetComponentOfTime(const TimeComponent timeComponent) noexcept;

		// ASCII
		std::string GetTimeText() noexcept;
		std::string GetDateInTextFormat(const bool fullDayname = true) noexcept;
		constexpr std::string_view GetMonth(const unsigned short month) noexcept;
		constexpr std::string_view GetDayOfTheWeek(const unsigned short day) noexcept;
		std::string GetDateInNumericalFormat(const bool slash = true) noexcept;
		std::string CreateTime(unsigned short hour, unsigned short minute, unsigned short second) noexcept;
		std::string GetLocalStartTimeStr() noexcept;
		std::string GetSystemStartTimeStr() noexcept;
		std::string GetLocalStartDateStr(const bool slash = true) noexcept;
		//std::string GetSystemStartDateStr(const bool slash = true) noexcept;


		// Wide Multi-Byte Chars
		std::wstring wGetTimeText() noexcept;
		std::wstring wGetDateInTextFormat(const bool fullDayname = true) noexcept;
		constexpr std::wstring_view wGetMonth(const unsigned short month) noexcept;
		constexpr std::wstring_view wGetDayOfTheWeek(const unsigned short day) noexcept;
		std::wstring wGetDateInNumericalFormat(const bool slash = true) noexcept;
		std::wstring wCreateTime(unsigned short hour, unsigned short minute, unsigned short second) noexcept;
		std::wstring wGetLocalStartTimeStr() noexcept;
		std::wstring wGetSystemStartTimeStr() noexcept;
		std::wstring wGetLocalStartDateStr(const bool slash = true) noexcept;
		//std::wstring wGetSystemStartDateStr(const bool slash = true) noexcept;
	}
}
