#include "pch.hpp"
#include "kMilliseconds.hpp"
#include "../../String/kToString.hpp"

namespace klib::kCalendar
{
	std::string Millisecond::ToString(const std::string_view& format) const
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

		ToStringImpl(format, 'u', noMatchFunc, matchFunc);
		return output;
	}

	std::string Millisecond::ToStringUsingTokenCount(const size_t count) const
	{
		const auto numberFormat = "{0:" + kString::ToString<char>(count) + "}";
		const auto milliStr = kString::ToString(numberFormat, GetMillisecond());
		return milliStr;
	}

	bool Millisecond::Verify() const
	{
		return millisecond < 1000;
	}

	void Millisecond::Limit()
	{
		millisecond %= 1000;
	}
}
