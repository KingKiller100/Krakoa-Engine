#pragma once

#include "kDate.hpp"

#include "../kComponentToStringImpl.hpp"

#include <array>
#include <string>

namespace klib::kCalendar
{
	class Date::Day : ComponentToStringImpl
	{
	public:
		enum DayOfTheWeek : unsigned char
		{
			SUNDAY = 0, MONDAY, TUESDAY,
			WEDNESDAY, THURSDAY, FRIDAY,
			SATURDAY,
		};

	public:
		constexpr Day(size_t day, DayOfTheWeek dotw = MONDAY)
			: day(day), dayOfTheWeek(dotw)
		{}
		
		~Day() noexcept = default;

		USE_RESULT constexpr size_t GetDay() const noexcept
		{
			return day;
		}

		USE_RESULT constexpr DayOfTheWeek GetDayOfTheWeek() const noexcept
		{
			return dayOfTheWeek;
		}

		USE_RESULT static constexpr auto DayOfTheWeekToString(DayOfTheWeek day)
		{
			constexpr std::array<const char*, 7> kCalendar_DaysOfTheWeek = {
				"Sunday", "Monday", "Tuesday", "Wednesday",
			"Thursday", "Friday", "Saturday"
			};

			return kCalendar_DaysOfTheWeek[day];
		}

		bool Verify() const;
		
		USE_RESULT std::string ToString(const std::string_view& format);

	private:
		USE_RESULT std::string GetDayStr() const;
		USE_RESULT std::string GetDayOfTheWeekStr() const;
		
	private:
		const size_t day;
		const DayOfTheWeek dayOfTheWeek;
	};


	constexpr Date::Day operator"" _d(unsigned long long day)
	{
		return Date::Day(static_cast<size_t>(day));
	}
}

