#pragma once

#include "kDate.hpp"
#include <cstdint>

#include "../kComponentToStringImpl.hpp"


namespace klib::kCalendar
{
	class Date::Year : private CalendarComponentToStringImpl
	{
	public:
		constexpr explicit Year(const std::uint16_t year)
			: year(year)
		, isLeapYear(year % 4 == 0)
		{}

		~Year() noexcept = default;

		USE_RESULT constexpr std::uint16_t GetYear() const
		{
			return year;
		}

		USE_RESULT constexpr bool IsLeapYear() const
		{
			return isLeapYear;
		}

		USE_RESULT std::string GetYearStr() const;
		USE_RESULT std::string ToString(const std::string_view& format) const;
		
	private:
		const std::uint16_t year;
		bool isLeapYear;
	};

	constexpr Date::Year operator"" _y(unsigned long long year)
	{
		return Date::Year(static_cast<std::uint16_t>(year));
	}
}
