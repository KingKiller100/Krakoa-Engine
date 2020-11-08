#pragma once
#include "kDate.hpp"
#include "../kComponentToStringImpl.hpp"
#include "kDay.hpp"

#include <array>
#include <string>

namespace klib::kCalendar
{
	class Date::Month : private CalendarComponentToStringImpl
	{
	public:
		enum MonthOfTheYear : unsigned char
		{
			JAN = 0, FEB, MAR,
			APR, MAY, JUN,
			JUL, AUG, SEP,
			OCT, NOV, DEC,
		};

	public:
		constexpr Month(MonthOfTheYear month)
			: moty(month)
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
			constexpr std::array<const char*, 12> kCalendar_MonthsArray = {
				"January", "February", "March",
				"April", "May", "June",
				"July", "August", "September",
				"October", "November", "December"
			};

			return kCalendar_MonthsArray[month];
		}

		USE_RESULT std::string ToString(const std::string_view& format) const;
		USE_RESULT bool Verify(const Day& day, const bool isLeapYear) const;

	private:
		USE_RESULT std::string GetMonthStr() const;


	private:
		const MonthOfTheYear moty;
	};

	constexpr Date::Month operator"" _m(unsigned long long month)
	{
		return Date::Month(static_cast<Date::Month::MonthOfTheYear>(month % 12));
	}
}
