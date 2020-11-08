#pragma once

#include "../kComponentToStringImpl.hpp"

#include "../../../HelperMacros.hpp"

#include <cstdint>

namespace klib::kCalendar
{	
	class Year : private CalendarComponentToStringImpl
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

		template<typename TargetType>
		constexpr operator TargetType() const
		{
			return GetYear();
		}
		
		USE_RESULT std::string GetYearStr() const;
		USE_RESULT std::string ToString(const std::string_view& format) const;
		
	private:
		const std::uint16_t year;
		bool isLeapYear;
	};

	constexpr Year operator"" _y(unsigned long long year)
	{
		return Year(static_cast<std::uint16_t>(year));
	}
}
