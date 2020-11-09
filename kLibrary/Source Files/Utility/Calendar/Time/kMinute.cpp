#include "pch.hpp"
#include "kMinute.hpp"

#include "../../String/kToString.hpp"

namespace klib::kCalendar
{
	std::string Minute::ToString(const std::string_view& format) const
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

	std::string Minute::ToStringUsingTokenCount(const size_t count) const
	{
		const auto numberFormat = "{0:" + kString::ToString<char>(count) + "}";
		const auto minStr = kString::ToString(numberFormat, GetMinute());
		return minStr;
	}

	bool Minute::Verify() const
	{
		return minute < 60;
	}

	void Minute::Limit()
	{
		minute %= 60;
	}
}
