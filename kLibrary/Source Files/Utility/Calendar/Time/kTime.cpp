#include "pch.hpp"
#include "kTime.hpp"
#include "../String/kToString.hpp"
#include "../Debug Helper/Exceptions/CalenderExceptions.hpp"
#include <Windows.h>

namespace klib::kCalendar
{
	namespace
	{
	}

	// TIME ////////////////////////////////////////////////////////////

	Time::Time(const _SYSTEMTIME& timeSource)
		: hours(timeSource.wHour)
		, minutes(timeSource.wMinute)
		, seconds(timeSource.wSecond)
		, milliseconds(timeSource.wMilliseconds)
	{}

	Time::Time(CalendarSourceType sourceType)
		: Time(GetDateAndTime(sourceType))
	{}

	Time::Time(const HHMMSSMS_t h, const HHMMSSMS_t m, const HHMMSSMS_t s, const HHMMSSMS_t ms)
		: hours(h)
		, minutes(m)
		, seconds(s)
		, milliseconds(ms)
	{
		CheckTime();
	}

	void Time::CheckTime() const
	{
		if (hours > 23)
			kDebug::CalendarError("Invalid Hours");

		if (minutes > 59)
			kDebug::CalendarError("Invalid Minutes");

		if (seconds > 59)
			kDebug::CalendarError("Invalid Seconds");

		if (milliseconds > 999)
			kDebug::CalendarError("Invalid Milliseconds");
	}

	Time::HHMMSSMS_t Time::GetComponent(const TimeComponent timeComponent) const
	{
		switch (timeComponent) {
		case TimeComponent::HOURS:		return hours;
		case TimeComponent::MINS:		return minutes;
		case TimeComponent::SECS:		return seconds;
		case TimeComponent::MILLIS:		return milliseconds;
		default: throw kDebug::CalendarError("Unknown time component");
		}
	}

	std::string Time::ToString(const TimeComponent accuracy) const
	{
		std::string str;
		HHMMSSMS_t times[] = { hours, minutes, seconds };

		for (auto i = CAST(HHMMSSMS_t, TimeComponent::HOURS);
			i <= CAST(HHMMSSMS_t, accuracy);
			++i)
		{
			if (!str.empty())
				str.push_back(':');
			str.append(i >= CAST(HHMMSSMS_t, TimeComponent::MILLIS)
				? kString::ToString("{0:3}", milliseconds)
				: kString::ToString("{0:2}", times[i]));
		}

		return str;
	}

	std::string Time::ToString(const std::string_view& format) const
	{
		using TokenT = char;

		constexpr auto noMatchToken = type_trait::s_NullTerminator<char>;
		constexpr std::array<TokenT, 4> tokens{ 'h', 'm', 's', 'u' };

		std::string output;
		output.reserve(format.size());

		size_t index = 0;
		for (auto letter = format.front();
			letter != type_trait::s_NullTerminator<char>;
			letter = format[++index])
		{
			TokenT match(noMatchToken);
			std::for_each(tokens.begin(), tokens.end(), [&](const TokenT pair)
				{
					if (pair == letter)
						match = pair;
				});

			if (match == type_trait::s_NullTerminator<char>)
			{
				output.push_back(letter);
				continue;
			}

			const auto first = format.find_first_of(match, index);
			auto last = format.find_first_not_of(match, first);

			if (last == std::string::npos)
				last = format.size();

			const auto count = last - first;
			const auto numberFormat = "{0:" + kString::ToString<char>("{0}", count) + "}";

			std::string toAdd;
			toAdd.reserve(count);

			switch (kString::ToLower(match))
			{
			case 'h':
				toAdd = kString::ToString(numberFormat, GetHours());
				break;
			case 'm':
				toAdd = kString::ToString(numberFormat, GetMinutes());
				break;
			case 's':
				toAdd = kString::ToString(numberFormat, GetSeconds());
				break;
			case 'u':
				toAdd = kString::ToString(numberFormat, GetMilliseconds());
				break;
			default:
				throw kDebug::CalendarError("Bad format for time");
				break;
			}
			output.append(std::move(toAdd));
			index += count - 1;

			if (index >= format.size() - 1)
				break;
		}
		return output;
	}

	Time::HHMMSSMS_t Time::GetHours() const
	{
		return hours;
	}

	Time::HHMMSSMS_t Time::GetMinutes() const
	{
		return minutes;
	}

	Time::HHMMSSMS_t Time::GetSeconds() const
	{
		return seconds;
	}

	Time::HHMMSSMS_t Time::GetMilliseconds() const
	{
		return milliseconds;
	}
}
