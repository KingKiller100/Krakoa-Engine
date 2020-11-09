#include "pch.hpp"
#include "kSecond.hpp"

#include "../../String/kToString.hpp"

namespace klib::kCalendar
{
	std::string Second::ToString( const std::string_view& format ) const
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

		ToStringImpl(format, 's', noMatchFunc, matchFunc);
		return output;
	}

	std::string Second::ToStringUsingTokenCount( const size_t count ) const
	{
		const auto numberFormat = "{0:" + kString::ToString<char>(count) + "}";
		const auto secStr = kString::ToString(numberFormat, GetSecond());
		return secStr;
	}

	bool Second::Verify() const
	{
		return second < 60;
	}

	void Second::Limit()
	{
		second %= 60;
	}
}
