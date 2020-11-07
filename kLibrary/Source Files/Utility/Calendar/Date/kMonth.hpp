#pragma once
#include "kDate.hpp"
#include <array>
#include <string>

#include "../kComponentToStringImpl.hpp"

namespace klib::kCalendar
{
	class Date::Month : ComponentToStringImpl
	{
	public:
		enum MonthOfTheYear : unsigned char
		{
			JAN = 0, FEB, MAR,
			APR, MAY, JUN,
			JUL, AUG, SEPT,
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
		USE_RESULT bool Verify() const;

	private:
		USE_RESULT std::string GetMonthStr() const;

	private:
		MonthOfTheYear moty;
	};
}
