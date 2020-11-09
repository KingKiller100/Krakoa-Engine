#include "pch.hpp"
#include "kMonth.hpp"
#include "kYear.hpp"

#include "../../String/kToString.hpp"

namespace klib::kCalendar
{
	Month Month::MonthFromDays(const std::uint16_t days)
	{
		const auto monthIndex = (days / MonthsInYear) % MonthsInYear;
		return Month(static_cast<MonthOfTheYear>(monthIndex));
	}

	std::string Month::ToString(const std::string_view& format) const
	{
		std::string output;

		const auto noMatchFunc = [&](char noToken)
		{
			output.push_back(noToken);
		};

		const auto matchFunc = [&](size_t count)
		{
			output.append(ToStringUsingTokenCount(count));
		};

		ToStringImpl(format, 'm', noMatchFunc, matchFunc);
		return output;
	}

	bool Month::Verify(const Day& day, const Year year) const
	{
		switch (moty)
		{
		case FEB: return year.IsLeapYear()
			? day.GetDay() <= 29
			: day.GetDay() <= 28;

		case JAN:
		case MAR:
		case MAY:
		case JUL:
		case AUG:
		case OCT:
		case DEC: return day.GetDay() <= 31;

		case APR:
		case JUN:
		case SEP:
		case NOV: return day.GetDay() <= 30;

		default: return false;
		}
	}

	std::string Month::GetMonthStr() const
	{
		return MonthToString(moty);
	}

	std::string Month::ToStringUsingTokenCount(const size_t count) const
	{
		if (count >= 4)
			return kString::ToString("{0}", GetMonthStr());
		if (count == 3)
			return  kString::ToString("{0}", GetMonthStr().substr(0, 3));
		if (count == 2)
			return kString::ToString("{0:2}", GetMonthNumber());

		return  kString::ToString("{0}", GetMonthNumber());
	}
}
