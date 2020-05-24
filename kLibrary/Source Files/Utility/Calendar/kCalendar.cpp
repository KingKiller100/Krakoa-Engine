#include "pch.hpp"
#include "kCalendar.hpp"

#include "../Format/kFormatToString.hpp"
#include "../String/kStringManipulation.hpp"


namespace klib::kCalendar
{
	using namespace kFormat;

	static const auto localStartTime = GetLocalDateAndTime();
	static const auto systemStartTime = GetSystemDateAndTime();

	_SYSTEMTIME& GetLocalDateAndTime()  noexcept
	{
		static _SYSTEMTIME kCalendar_Local_DateTime;
		GetLocalTime(&kCalendar_Local_DateTime);
		return kCalendar_Local_DateTime;
	}

	_SYSTEMTIME& GetSystemDateAndTime()  noexcept
	{
		static _SYSTEMTIME kCalendar_System_DateTime;
		GetSystemTime(&kCalendar_System_DateTime);
		return kCalendar_System_DateTime;
	}

	unsigned short GetComponentOfTime(const TimeComponent timeComponent)
	{
		const auto now = GetLocalDateAndTime();

		switch (timeComponent) {
		case TimeComponent::HOURS:		return now.wHour;
		case TimeComponent::MINS:		return now.wMinute;
		case TimeComponent::SECS:		return now.wSecond;
		case TimeComponent::MILLIS:		return now.wMilliseconds;
		default: throw kDebug::CalendarError();
		}
	}

	// ASCII

	std::string GetTimeText(const _SYSTEMTIME dateTime)  noexcept
	{
		const auto dateStr = ToString("{0:2}:{1:2}:{2:2}:{3:3}",
			dateTime.wHour,
			dateTime.wMinute,
			dateTime.wSecond,
			dateTime.wMilliseconds);
		return dateStr;
	}

	std::string GetDateInTextFormat(const DateFormat format, const _SYSTEMTIME& dateTime) noexcept
	{
		std::string day = GetDayOfTheWeek(dateTime.wDayOfWeek).data();

		if (format == DateFormat::SHORT) day = day.substr(0, 3);
		const auto dateStr = ToString("%s %d %s %04d",
			day.data(),
			dateTime.wDay,
			GetMonth(dateTime.wMonth - 1).data(),
			dateTime.wYear);
		return dateStr;
	}

	std::string GetDateInNumericalFormat(const DateSeparator separator, const _SYSTEMTIME& dateTime) noexcept
	{
		const auto* const dateFormat = (separator == DateSeparator::SLASH) ? "%02d/%02d/%02d" : "%02d-%02d-%04d";
		return ToString(dateFormat, dateTime.wDay, dateTime.wMonth, dateTime.wYear);
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
		static const auto localTimeStr =
			ToString("%02d:%02d:%02d:%03d",
				localStartTime.wHour,
				localStartTime.wMinute,
				localStartTime.wSecond,
				localStartTime.wMilliseconds);
		return localTimeStr;
	}

	std::string GetSystemStartTimeStr() noexcept
	{
		static const auto systemTimeStr =
			ToString("%02d:%02d:%02d:%03d",
				systemStartTime.wHour,
				systemStartTime.wMinute,
				systemStartTime.wSecond,
				systemStartTime.wMilliseconds);
		return systemTimeStr;
	}

	std::string GetLocalStartDateStr(const DateSeparator separator) noexcept
	{
		static std::string local;
		if (local.empty())
		{
			local = GetDateInNumericalFormat(separator, GetLocalDateAndTime());
			return local;
		}

		if (separator == DateSeparator::SLASH)
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

	std::string GetSystemStartDateStr(const DateSeparator separator) noexcept
	{
		static std::string system;
		if (system.empty())
		{
			system = GetDateInNumericalFormat(separator, GetSystemDateAndTime());
			return system;
		}

		if (separator == DateSeparator::SLASH)
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

	// WIDE MULTI-BYTE CHAR

	std::wstring wGetTimeText(const _SYSTEMTIME dateTime)  noexcept
	{
		return ToString(L"%02d:%02d:%02d:%03d", dateTime.wHour, dateTime.wMinute, dateTime.wSecond, dateTime.wMilliseconds);
	}

	std::wstring wGetDateInTextFormat(const DateFormat format, const _SYSTEMTIME& dateTime) noexcept
	{
		std::wstring day = wGetDayOfTheWeek(dateTime.wDayOfWeek).data();
		if (format == DateFormat::SHORT) day = day.substr(0, 3);
		return ToString(L"%s %d %s %04d", day.data(), dateTime.wDay, wGetMonth(dateTime.wMonth - 1).data(), dateTime.wYear);
	}

	std::wstring wGetDateInNumericalFormat(const DateSeparator separator, const _SYSTEMTIME& dateTime) noexcept
	{
		const auto* const dateFormat = (separator == DateSeparator::SLASH) ? L"%02d/%02d/%02d" : L"%02d-%02d-%04d";
		return ToString(dateFormat, dateTime.wDay, dateTime.wMonth, dateTime.wYear);
	}

	std::wstring wCreateTime(uint8_t hours, uint8_t minutes) noexcept
	{
		if (hours > 23) hours = 23;
		if (minutes > 59) minutes = 59;
		const auto time = ToString(L"%02d:%02d", hours, minutes);
		return time;
	}

	std::wstring wCreateTime(uint8_t hours, uint8_t minutes, uint8_t seconds) noexcept
	{
		if (hours > 23) hours = 23;
		if (minutes > 59) minutes = 59;
		if (seconds > 59) seconds = 59;
		const auto time = ToString(L"%02d:%02d:%02d", hours, minutes, seconds);
		return time;
	}

	std::wstring wCreateTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds) noexcept
	{
		if (hours > 23) hours = 23;
		if (minutes > 59) minutes = 59;
		if (seconds > 59) seconds = 59;
		if (milliseconds > 999) milliseconds = 999;
		const auto time = ToString(L"%02d:%02d:%02d:%03d", hours, minutes, seconds, milliseconds);
		return time;
	}

	std::wstring wGetLocalStartTimeStr() noexcept
	{
		static const auto localTimeStr =
			ToString(L"%02d:%02d:%02d:%03d",
				localStartTime.wHour,
				localStartTime.wMinute,
				localStartTime.wSecond,
				localStartTime.wMilliseconds);
		return localTimeStr;
	}

	std::wstring wGetSystemStartTimeStr() noexcept
	{
		static const auto systemTimeStr =
			ToString(L"%02d:%02d:%02d:%03d",
				systemStartTime.wHour,
				systemStartTime.wMinute,
				systemStartTime.wSecond,
				systemStartTime.wMilliseconds);
		return systemTimeStr;
	}

	std::wstring wGetLocalStartDateStr(const DateSeparator separator) noexcept
	{
		static std::wstring local;
		if (local.empty())
		{
			local = wGetDateInNumericalFormat(separator, GetLocalDateAndTime());
			return local;
		}

		if (separator == DateSeparator::SLASH)
		{
			if (local.find(L'/') == std::wstring::npos)
				local = kString::Replace<wchar_t>(local, L'-', L'/');
		}
		else
		{
			if (local.find(L'-') == std::wstring::npos)
				local = kString::Replace<wchar_t>(local, L'/', L'-');
		}

		return local;
	}

	std::wstring wGetSystemLocalStartDateStr(const DateSeparator separator) noexcept
	{
		static std::wstring system;
		if (system.empty())
		{
			system = wGetDateInNumericalFormat(separator, GetSystemDateAndTime());
			return system;
		}

		if (separator == DateSeparator::SLASH)
		{
			if (system.find(L'/') == std::wstring::npos)
				system = kString::Replace<wchar_t>(system, L'-', L'/');
		}
		else
		{
			if (system.find('-') == std::wstring::npos)
				system = kString::Replace<wchar_t>(system, L'/', L'-');
		}

		return system;
	}
}


