#pragma once
#include "kDay.hpp"
#include "kMonth.hpp"
#include "kYear.hpp"

#include "../../../HelperMacros.hpp"

struct _SYSTEMTIME;

namespace klib
{
	namespace kCalendar
	{
		enum class CalendarInfoSourceType;

		class Date
		{
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

		public:
			explicit Date(CalendarInfoSourceType sourceType);
			Date(const Day::DayOfTheWeek dayOfTheWeek, const std::uint16_t d = 1, const Month::MonthOfTheYear m = kCalendar::Month::JAN, const std::uint16_t y = 1900);

			USE_RESULT std::string ToString(const std::string_view& format) const;
			USE_RESULT std::string ToString(DateNumericalSeparator separator = DateNumericalSeparator::SLASH) const;
			USE_RESULT std::string ToString(DateTextLength format) const;

			USE_RESULT const Day& GetDay() const;
			USE_RESULT Day& GetDay();
			USE_RESULT const Month& GetMonth() const;
			USE_RESULT Month& GetMonth();
			USE_RESULT const Year& GetYear() const;
			USE_RESULT Year& GetYear();

		private:
			void CheckDate() const;
			Date(const _SYSTEMTIME& dateSource);

		private:
			const Day day;
			const Month monthIndex;
			const Year year;
		};



	}
}