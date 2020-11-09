#pragma once

#include "../../../HelperMacros.hpp"

#include "../kComponentToStringImpl.hpp"

#include <cstdint>

namespace klib::kCalendar
{	
	class Year final : private CalendarComponentToStringImpl
	{
		static constexpr auto daysInYear = 365;
		static constexpr auto daysInLeapYear = 366;
		static constexpr auto leapYearFrequency = 4;
		
		static constexpr auto MonthsInYear = 12;
		
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

		static Year FromDays(const std::uint16_t days);
		static Year FromMonths(const std::uint16_t months);
		
		USE_RESULT std::string GetYearStr() const;
		USE_RESULT std::string ToString(const std::string_view& format) const;

		friend class Date;

	protected:
		USE_RESULT std::string ToStringUsingTokenCount( const size_t count ) const override;
		
	private:
		const std::uint16_t year;
		const bool isLeapYear;
	};

	constexpr Year operator"" _y(unsigned long long year)
	{
		return Year(static_cast<std::uint16_t>(year));
	}
}
