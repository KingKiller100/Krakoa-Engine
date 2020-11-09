#include "pch.hpp"
#include "kTime.hpp"
#include "../kiCalendarInfoSource.hpp"
#include "../../String/kToString.hpp"
#include "../../Debug Helper/Exceptions/CalenderExceptions.hpp"


namespace klib::kCalendar
{
	namespace
	{
		using namespace secret::helper;

		iCalendarInfoSource& GetInfoSource(CalendarInfoSourceType type)
		{
			static iCalendarInfoSource& calendar_info = GetCalendarInfoSource();
			calendar_info.Refresh(type);
			return calendar_info;
		}
	}

	// TIME ////////////////////////////////////////////////////////////

	Time::Time(CalendarInfoSourceType sourceType)
		: Time(GetInfoSource(sourceType))
	{}

	Time::Time(const iCalendarInfoSource& source)
		: Time(Hour::CycleType::CYCLE_24
			, source.GetHour()
			, source.GetMinute()
			, source.GetSecond()
			, source.GetMillisecond())
	{}

	Time::Time(const Hour::CycleType cycle, const std::uint16_t h, const std::uint16_t m, const std::uint16_t s,
		const std::uint16_t ms)
		: hour(h, cycle)
		, minute(m)
		, second(s)
		, millisecond(ms)
	{
		CheckTime();
	}

	std::uint16_t Time::GetComponent(const TimeComponent timeComponent) const
	{
		switch (timeComponent) {
		case TimeComponent::HOURS:		return hour;
		case TimeComponent::MINS:		return minute;
		case TimeComponent::SECS:		return second;
		case TimeComponent::MILLIS:		return millisecond;
		default: throw kDebug::CalendarError("Unknown time component");
		}
	}

	void Time::CheckTime() const
	{
		if (!hour.Verify())
			kDebug::CalendarError("Invalid Hours");

		if (!minute.Verify())
			kDebug::CalendarError("Invalid Minutes");

		if (!second.Verify())
			kDebug::CalendarError("Invalid Seconds");

		if (!millisecond.Verify())
			kDebug::CalendarError("Invalid Milliseconds");
	}

	std::string Time::ToString(const TimeComponent accuracy) const
	{
		std::string str;
		std::uint16_t times[] = { hour, minute, second };

		for (auto i = CAST(std::uint16_t, TimeComponent::HOURS);
			i <= CAST(std::uint16_t, accuracy);
			++i)
		{
			if (!str.empty())
				str.push_back(':');
			str.append(i >= CAST(std::uint16_t, TimeComponent::MILLIS)
				? kString::ToString("{0:3}", millisecond)
				: kString::ToString("{0:2}", times[i]));
		}

		return str;
	}

	std::string Time::ToString(const std::string_view& format) const
	{
		constexpr auto noMatchToken = type_trait::s_NullTerminator<char>;

		const std::set<char> tokens{ Hour::FormatToken, Minute::FormatToken, Second::FormatToken, Millisecond::FormatToken };

		std::string output;

		const auto noMatchFunc = [&](char noToken)
		{
			output.push_back(noToken);
		};

		const auto matchFunc = [&](size_t count, char token)
		{
			switch (kString::ToLower(token))
			{
			case Hour::FormatToken:
				output.append(hour.ToStringUsingTokenCount(count));
				break;

			case Minute::FormatToken:
				output.append(minute.ToStringUsingTokenCount(count));
				break;

			case Second::FormatToken:
				output.append(second.ToStringUsingTokenCount(count));
				break;

			case Millisecond::FormatToken:
				output.append(millisecond.ToStringUsingTokenCount(count));
				break;

			default:
				throw kDebug::CalendarError("Bad format for date");
			}
		};

		ToStringImpl(format, tokens, noMatchFunc, matchFunc);
		return output;
	}

	Hour& Time::GetHour()
	{
		return hour;
	}

	const Hour& Time::GetHour() const
	{
		return hour;
	}

	Minute& Time::GetMinute()
	{
		return minute;
	}

	const Minute& Time::GetMinute() const
	{
		return minute;
	}

	Second& Time::GetSecond()
	{
		return second;
	}

	const Second& Time::GetSecond() const
	{
		return second;
	}

	Millisecond& Time::GetMillisecond()
	{
		return millisecond;
	}

	const Millisecond& Time::GetMillisecond() const
	{
		return millisecond;
	}
}
