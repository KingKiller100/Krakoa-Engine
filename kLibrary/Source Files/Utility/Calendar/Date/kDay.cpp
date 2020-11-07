#include "pch.hpp"
#include "kDay.hpp"

#include "../../String/kToString.hpp"

#include <algorithm>

namespace klib::kCalendar
{
	bool Date::Day::Verify() const
	{
		return day <= 31;
	}

	std::string Date::Day::ToString(const std::string_view& format)
	{
		return operator()(format, 'd', [&](size_t count, std::string& finalString)
			{
				std::string toAppend;
				toAppend.reserve(count);
				if (count >= 5)
					toAppend = kString::ToString("{0}", GetDayOfTheWeekStr());
				else if (count == 4)
					toAppend = kString::ToString("{0}", GetDayOfTheWeekStr().substr(0, 3));
				else if (count == 3)
					toAppend = kString::ToString("{0}", GetDayStr());
				else if (count == 2)
					toAppend = kString::ToString("{0:2}", GetDay());
				else
					toAppend = kString::ToString("{0}", GetDay());
				finalString.append(std::move(toAppend));
			});
	}

	std::string Date::Day::GetDayStr() const
	{
		const auto dateSuffix = [&]()
		{
			return (day == 1 || day == 21 || day == 31) ? "st"
				: (day == 2 || day == 22) ? "nd"
				: day == 3 ? "rd"
				: "th";
		};
		const auto str = kString::ToString("{0}{1}"
			, day
			, dateSuffix());
		return str;
	}

	std::string Date::Day::GetDayOfTheWeekStr() const
	{
		return DayOfTheWeekToString(dayOfTheWeek);
	}
}
