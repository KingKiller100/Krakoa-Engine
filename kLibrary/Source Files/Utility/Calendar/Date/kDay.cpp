#include "pch.hpp"
#include "kDay.hpp"

#include "../../String/kToString.hpp"

namespace klib::kCalendar
{
	bool Day::Verify() const
	{
		return day > 0 && day <= 31;
	}

	std::string Day::ToString(const std::string_view& format) const
	{
		std::string output;

		const auto noMatchFunc = [&](char notToken)
		{
			output.push_back(notToken);
		};

		const auto matchFunc = [&](size_t count)
		{
			output.append(ToStringUsingTokenCount(count));
		};

		ToStringImpl(format, 'd'
			, noMatchFunc
			, matchFunc);

		return output;
	}

	std::string Day::GetDayStr() const
	{
		const auto str = kString::ToString("{0}{1}"
			, day
			, GetDaySuffix(day));
		return str;
	}

	std::string_view Day::GetDaySuffix(const std::uint16_t day)
	{
		return (day == 1 || day == 21 || day == 31) ? "st"
			: (day == 2 || day == 22) ? "nd"
			: day == 3 ? "rd"
			: "th";
	}

	std::string Day::GetDayOfTheWeekStr() const
	{
		return DayOfTheWeekToString(dayOfTheWeek);
	}

	std::string Day::ToStringUsingTokenCount(const size_t count) const
	{
		if (count >= 5)
			return kString::ToString("{0}", GetDayOfTheWeekStr());
		if (count == 4)
			return  kString::ToString("{0}", GetDayOfTheWeekStr().substr(0, 3));
		if (count == 3)
			return  kString::ToString("{0}", GetDayStr());
		if (count == 2)
			return  kString::ToString("{0:2}", GetDay());

		return  kString::ToString("{0}", GetDay());
	}
}
