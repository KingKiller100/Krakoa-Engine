#include "pch.hpp"
#include "kHour.hpp"

#include "../../String/kToString.hpp"

namespace klib::kCalendar
{
	std::string Hour::ToString(const std::string_view& format) const
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

		ToStringImpl(format, 'h', noMatchFunc, matchFunc);
		return output;
	}

	std::string Hour::ToStringUsingTokenCount(const size_t count) const
	{
		const auto numberFormat = "{0:" + kString::ToString<char>(count) + "}";
		const auto hourStr = kString::ToString(numberFormat, GetHour());
		return hourStr;
	}

	bool Hour::Verify() const
	{
		switch (cycleType)
		{
		case CYCLE_12: return hour < 12;
		case CYCLE_24: return hour < 24;
		default: return false;
		}
	}

	void Hour::Limit()
	{
		switch (cycleType)
		{
		case CYCLE_12: hour %= 12;
		case CYCLE_24: hour %= 24;
		default: ;
		}
	}
}
