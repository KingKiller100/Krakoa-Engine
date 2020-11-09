#pragma once

#include "../kComponentToStringImpl.hpp"
#include "../../../HelperMacros.hpp"

#include <array>
#include <string>

namespace klib::kCalendar
{
	class Day : private CalendarComponentToStringImpl
	{
	public:
		enum DayOfTheWeek : unsigned char
		{
			SUNDAY = 0, MONDAY, TUESDAY,
			WEDNESDAY, THURSDAY, FRIDAY,
			SATURDAY,
		};

	public:
		constexpr Day(std::uint16_t day, DayOfTheWeek dotw = MONDAY)
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

		static std::string_view GetDaySuffix(const std::uint16_t day);
		
		USE_RESULT bool Verify() const;
		
		USE_RESULT std::string ToString(const std::string_view& format) const;

		template<typename TargetType>
		constexpr operator TargetType() const
		{
			return GetDay();
		}

		friend class Date;
		
	private:
		USE_RESULT std::string GetDayStr() const;
		USE_RESULT std::string GetDayOfTheWeekStr() const;
		USE_RESULT std::string ToStringUsingTokenCount(const size_t count) const override;
		
	private:
		const std::uint16_t day;
		const DayOfTheWeek dayOfTheWeek;
	};


	constexpr Day operator"" _d(unsigned long long day)
	{
		return Day(static_cast<std::uint16_t>(day));
	}
}

