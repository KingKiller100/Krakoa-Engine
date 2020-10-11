#pragma once

#include <cstdint>
#include <string>

#include "../../HelperMacros.hpp"


#include "../Debug Helper/Exceptions/CalenderExceptions.hpp"

namespace klib
{
	namespace kCalendar
	{
		enum class CalendarInfoSource
		{
			SYSTEM, LOCAL,
		};
		
		enum class TimeComponent : uint8_t
		{
			HOURS,
			MINS,
			SECS,
			MILLIS,
		};
		
		enum class DateTextLength : uint8_t
		{
			FULL,
			SHORT
		};

		enum class DateNumericalSeparator : uint8_t
		{
			SLASH,
			DASH,
		};

		enum DaysOfTheWeek : unsigned char
		{
			SUNDAY = 0,
			MONDAY,
			TUESDAY,
			WEDNESDAY,
			THURSDAY,
			FRIDAY,
			SATURDAY,
		};

		class Date
		{
			using DDMMYYYY_t = std::uint16_t;
			
		public:
			explicit Date(CalendarInfoSource sourceInfo = CalendarInfoSource::LOCAL);
			Date(const DaysOfTheWeek dayOfTheWeek, const DDMMYYYY_t d, const DDMMYYYY_t m, const DDMMYYYY_t y);
			
			USE_RESULT std::string NumericalFormat(DateNumericalSeparator separator) const;
			USE_RESULT std::string TextFormat(DateTextLength format) const;

			USE_RESULT DDMMYYYY_t GetDay() const;
			USE_RESULT std::string GetDayStr() const;
			USE_RESULT DDMMYYYY_t GetMonth() const;
			USE_RESULT std::string GetMonthStr() const;
			USE_RESULT DDMMYYYY_t GetYear() const;
			USE_RESULT std::string GetYearStr() const;
			USE_RESULT DaysOfTheWeek GetDayOfWeek() const;
			USE_RESULT std::string GetDayOfWeekStr() const;

		private:
			void CheckDate() const;
			
		private:
			DDMMYYYY_t day;
			DDMMYYYY_t month;
			DDMMYYYY_t year;
			DaysOfTheWeek dayOfWeek;
		};

		class Time
		{
			using HHMMSSMS_t = std::uint16_t;
			
		public:
			explicit Time(CalendarInfoSource sourceInfo = CalendarInfoSource::LOCAL);
			Time(const HHMMSSMS_t h, const HHMMSSMS_t m, const HHMMSSMS_t s,
				const HHMMSSMS_t ms = 0);

			USE_RESULT HHMMSSMS_t GetComponent(const TimeComponent timeComponent) const;
			USE_RESULT std::string ToString(const TimeComponent accuracy) const;

			USE_RESULT HHMMSSMS_t GetHours() const;
			USE_RESULT HHMMSSMS_t GetMinutes() const;
			USE_RESULT HHMMSSMS_t GetSeconds() const;
			USE_RESULT HHMMSSMS_t GetMilliseconds() const;
			
		private:
			void CheckTime() const;

			private:
			HHMMSSMS_t hours;
			HHMMSSMS_t minutes;
			HHMMSSMS_t seconds;
			HHMMSSMS_t milliseconds;
		};
	}

#ifdef KLIB_SHORTHAND_NAMESPACE
		using namespace kCalendar;
#endif
}
