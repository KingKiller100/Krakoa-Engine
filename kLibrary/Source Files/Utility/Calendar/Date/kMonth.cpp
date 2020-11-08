#include "pch.hpp"
#include "kMonth.hpp"

#include "../../String/kToString.hpp"

namespace klib::kCalendar
{
	std::string Date::Month::ToString(const std::string_view& format) const
	{
		std::string output;

		const auto noMatchFunc = [&](char noToken)
		{
			output.push_back(noToken);
		};

		const auto matchFunc = [&](size_t count)
		{
			std::string toAppend;
			if (count >= 4)
				toAppend = kString::ToString("{0}", GetMonthStr());
			else if (count == 3)
				toAppend = kString::ToString("{0}", GetMonthStr().substr(0, 3));
			else if (count == 2)
				toAppend = kString::ToString("{0:2}", GetMonth());
			else
				toAppend = kString::ToString("{0}", GetMonth());
			output.append(toAppend);
		};

		ToStringImpl(format, 'm', noMatchFunc, matchFunc);
		return output;
	}

	bool Date::Month::Verify(const Day& day, const bool isLeapYear) const
	{
		switch (moty)
		{
		case JAN: return day.GetDay() <= 31;
		case FEB: return isLeapYear ? day.GetDay() <= 29 : day.GetDay() <= 28;
		case MAR: return day.GetDay() <= 31;
		case APR: return day.GetDay() <= 30;
		case MAY: return day.GetDay() <= 31;
		case JUN: return day.GetDay() <= 30;
		case JUL: return day.GetDay() <= 31;
		case AUG: return day.GetDay() <= 31;
		case SEP: return day.GetDay() <= 30;
		case OCT: return day.GetDay() <= 31;
		case NOV: return day.GetDay() <= 30;
		case DEC: return day.GetDay() <= 31;
		default: return false;
		}
	}

	std::string Date::Month::GetMonthStr() const
	{
		return MonthToString(moty);
	}
}
