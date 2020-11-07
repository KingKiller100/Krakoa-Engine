#pragma once

#include <cstdint>
#include <string>

#include "../../HelperMacros.hpp"


struct _SYSTEMTIME;

namespace klib
{
	namespace kCalendar
	{
		enum class CalendarSourceType
		{
			SYSTEM, LOCAL,
		};

		class Date
		{
			using DDMMYYYY_t = std::uint16_t;

		public:
			enum DateTextLength : uint8_t
			{
				FULL,
				SHORT
			};

			enum DateNumericalSeparator : char
			{
				SLASH = '/',
				DASH = '-',
			};
			
			enum DaysOfTheWeek : unsigned char
			{
				SUNDAY = 0, MONDAY, TUESDAY,
				WEDNESDAY, THURSDAY, FRIDAY,
				SATURDAY,
			};

			enum MonthOfTheYear : unsigned char
			{
				JAN = 0, FEB, MAR,
				APR, MAY, JUN,
				JUL, AUG, SEPT,
				OCT, NOV, DEC,
			};
			
		public:
			explicit Date(CalendarSourceType sourceType);
			Date(const DaysOfTheWeek dayOfTheWeek, const DDMMYYYY_t d = 1, const MonthOfTheYear m = JAN, const DDMMYYYY_t y = 1900);

			USE_RESULT std::string ToString(const std::string_view& format) const;
			USE_RESULT std::string ToString(DateNumericalSeparator separator = DateNumericalSeparator::SLASH) const;
			USE_RESULT std::string ToString(DateTextLength format) const;

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
			Date(const _SYSTEMTIME& dateSource);
			
		private:
			const DDMMYYYY_t day;
			const MonthOfTheYear monthIndex;
			const DDMMYYYY_t year;
			const DaysOfTheWeek dayOfWeek;
		};

		class Time
		{
			using HHMMSSMS_t = std::uint16_t;

		public:
			enum TimeComponent : uint8_t
			{
				HOURS,
				MINS,
				SECS,
				MILLIS,
				COMPONENT_END_MARKER
			};
			
		public:
			explicit Time(CalendarSourceType sourceType);
			Time(const HHMMSSMS_t h, const HHMMSSMS_t m, const HHMMSSMS_t s,
				const HHMMSSMS_t ms = 0);

			USE_RESULT HHMMSSMS_t GetComponent(const TimeComponent timeComponent) const;
			USE_RESULT std::string ToString(const TimeComponent accuracy = MILLIS) const;
			USE_RESULT std::string ToString(const std::string_view& format) const;

			USE_RESULT HHMMSSMS_t GetHours() const;
			USE_RESULT HHMMSSMS_t GetMinutes() const;
			USE_RESULT HHMMSSMS_t GetSeconds() const;
			USE_RESULT HHMMSSMS_t GetMilliseconds() const;
			
		private:
			void CheckTime() const;

			explicit Time(const _SYSTEMTIME& timeSource);
			
		private:
			const HHMMSSMS_t hours;
			const HHMMSSMS_t minutes;
			const HHMMSSMS_t seconds;
			const HHMMSSMS_t milliseconds;
		};
	}

#ifdef KLIB_SHORT_NAMESPACE
		using namespace kCalendar;
#endif
}
