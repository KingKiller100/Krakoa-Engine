#pragma once

#include "../kComponentToStringImpl.hpp"
#include "kDay.hpp"
#include "kYear.hpp"

#include <array>
#include <string>

namespace klib::kCalendar
{
	class Month final : private CalendarComponentToStringImplExtended
	{
	public:
		enum MonthOfTheYear : unsigned char
		{
			JAN = 0, FEB, MAR,
			APR, MAY, JUN,
			JUL, AUG, SEP,
			OCT, NOV, DEC,
		};

		static constexpr auto FormatToken = 'm';
		static constexpr size_t MonthsInYear = 12;
		
	public:
		constexpr explicit Month(MonthOfTheYear month)
			: moty(month)
		{}
		
		constexpr explicit Month(const std::uint16_t days)
			: moty( MonthFromDays( days ) )
		{}

		~Month() = default;

		USE_RESULT constexpr MonthOfTheYear GetMonth() const
		{
			return moty;
		}

		USE_RESULT constexpr std::uint16_t GetMonthNumber() const
		{
			return static_cast<std::uint16_t>(moty) + 1;
		}

		USE_RESULT static constexpr const char* MonthToString(MonthOfTheYear month)
		{
			constexpr std::array<const char*, MonthsInYear> kCalendar_MonthsArray = {
				"January", "February", "March",
				"April", "May", "June",
				"July", "August", "September",
				"October", "November", "December"
			};

			return kCalendar_MonthsArray[month];
		}

		template<typename TargetType>
		constexpr operator TargetType() const
		{
			return static_cast<TargetType>(GetMonthNumber());
		}

		USE_RESULT static Month MonthFromDays(const std::uint16_t days);
		
		USE_RESULT std::string ToString(const std::string_view& format) const;
		USE_RESULT bool Verify(const Day& day, const Year year) const;

		friend class Date;
		
	protected:
		USE_RESULT std::string GetMonthStr() const;
		USE_RESULT std::string ToStringUsingTokenCount( const size_t count ) const override;
		
	private:
		const MonthOfTheYear moty;
	};

	constexpr Month operator"" _m(unsigned long long month)
	{
		return Month (static_cast<std::uint16_t>(month));
	}
}
