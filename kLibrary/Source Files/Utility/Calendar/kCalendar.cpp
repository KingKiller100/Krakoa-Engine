#include "pch.hpp"
#include "kCalendar.hpp"
#include "kDateTime.hpp"

#include "../Format/kFormatToString.hpp"
#include "../String/kStringManipulation.hpp"

#include <unordered_map>

namespace klib::kCalendar
{
	using namespace kFormat;

	static std::unordered_map<CalendarInfoSource, Date> dates = {
		 { CalendarInfoSource::LOCAL, Date(CalendarInfoSource::LOCAL) },
		 { CalendarInfoSource::SYSTEM, Date(CalendarInfoSource::SYSTEM) }
	};
	static std::unordered_map<CalendarInfoSource, Time> times = {
		 { CalendarInfoSource::LOCAL, Time(CalendarInfoSource::LOCAL) },
		 { CalendarInfoSource::SYSTEM, Time(CalendarInfoSource::SYSTEM) }
	};

	unsigned short GetComponentOfTime(const Time::TimeComponent timeComponent, const CalendarInfoSource source)
	{
		const auto now = Time(source);
		return now.GetComponent(timeComponent);
	}

	std::string GetTimeText(CalendarInfoSource calendarInfo)  noexcept
	{
		const auto now = Time(calendarInfo);
		return now.ToString(Time::TimeComponent::MILLIS);
	}

	std::string GetDateInTextFormat(const Date::DateTextLength format, const CalendarInfoSource source) noexcept
	{
		const auto date = Date(source);
		return date.ToString(format);
	}

	std::string GetDateInNumericalFormat(const Date::DateNumericalSeparator separator, const CalendarInfoSource source) noexcept
	{
		const auto date = Date(source);
		return date.ToString(separator);
	}

	std::string CreateTime(uint8_t hours, uint8_t minutes) noexcept
	{
		if (hours > 23) hours = 23;
		if (minutes > 59) minutes = 59;
		const auto time = ToString("%02d:%02d", hours, minutes);
		return time;
	}

	std::string CreateTime(uint8_t hours, uint8_t minutes, uint8_t seconds)  noexcept
	{
		if (hours > 23) hours = 23;
		if (minutes > 59) minutes = 59;
		if (seconds > 59) seconds = 59;
		const auto time = ToString("%02d:%02d:%02d", hours, minutes, seconds);
		return time;
	}

	std::string CreateTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds) noexcept
	{
		if (milliseconds > 999) milliseconds = 999;
		auto time = CreateTime(hours, minutes, seconds);
		time.append(ToString(":%03d", milliseconds));
		return time;
	}

	std::string GetLocalStartTimeStr() noexcept
	{
		static const auto& info = times.at(CalendarInfoSource::LOCAL);
		static const auto str =  info.ToString(Time::TimeComponent::MILLIS);
		return str;
	}

	std::string GetSystemStartTimeStr() noexcept
	{
		static const auto& info = times.at(CalendarInfoSource::SYSTEM);
		static const auto str = info.ToString(Time::TimeComponent::MILLIS);
		return str;
	}

	std::string GetLocalStartDateStr(const Date::DateNumericalSeparator separator) noexcept
	{
		static std::string local;
		if (local.empty())
		{
			static const auto& info = dates.at(CalendarInfoSource::LOCAL);
			local = info.ToString(separator);
			return local;
		}

		if (separator == Date::DateNumericalSeparator::SLASH)
		{
			if (local.find('/') == std::string::npos)
				local = kString::Replace<char>(local, '-', '/');
		}
		else
		{
			if (local.find('-') == std::string::npos)
				local = kString::Replace<char>(local, '/', '-');
		}

		return local;
	}

	std::string GetSystemStartDateStr(const Date::DateNumericalSeparator separator) noexcept
	{
		static std::string system;
		if (system.empty())
		{
			static const auto& info = dates.at(CalendarInfoSource::SYSTEM);
			system = info.ToString(separator);
			return system;
		}

		if (separator == Date::DateNumericalSeparator::SLASH)
		{
			if (system.find('/') == std::string::npos)
				system = kString::Replace<char>(system, '-', '/');
		}
		else
		{
			if (system.find('-') == std::string::npos)
				system = kString::Replace<char>(system, '/', '-');
		}

		return system;
	}
}


