#include "pch.hpp"
#include "kMonth.hpp"

namespace klib::kCalendar
{
	std::string Date::Month::ToString(const std::string_view& format) const
	{
	}

	std::string Date::Month::GetMonthStr() const
	{
		return MonthToString(moty);
	}
}
