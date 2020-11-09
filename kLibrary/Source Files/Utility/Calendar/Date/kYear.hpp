#pragma once

#include "../../../HelperMacros.hpp"

#include "../kComponentToStringImpl.hpp"

#include <cstdint>

#include "kDay.hpp"

namespace klib::kCalendar
{	
	class Year final : private CalendarComponentToStringImpl
	{
	public:
		static constexpr size_t LeapYearFrequency = 4;
		
		static constexpr size_t YearsInDecade = 10;
		static constexpr size_t YearsInCentury = 100;
		static constexpr size_t YearsInMillennium = 1000;
		
	public:
		constexpr explicit Year(const std::uint16_t year)
			: year(year)
		, isLeapYear((year % 4 == 0 // Year divisible by 4
		&& year % 100 != 0) // but not divisible by 100
		|| year % 400 == 0) // Unless it's divisible by 400
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

		USE_RESULT constexpr std::uint16_t TotalDays() const
		{
			return isLeapYear
				? Day::DaysInLeapYear
				: Day::DaysInYear;
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
