#pragma once
#include "../../../HelperMacros.hpp"

struct _SYSTEMTIME;

namespace klib
{
	namespace kCalendar
	{
		enum class CalendarInfoSourceType;

		class Date
		{
			using DDMMYYYY_t = std::uint16_t;
		public:
			class Day;
			class Month;
			class Year;

			
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

		public:
			explicit Date(CalendarInfoSourceType sourceType);
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



	}
}