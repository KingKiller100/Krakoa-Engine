#pragma once

#include "../../../HelperMacros.hpp"

#include "../kCalenderInfoSourceType.hpp"

#include <cstdint>
#include <string>


#include "kHour.hpp"
#include "kMinute.hpp"
#include "kSecond.hpp"
#include "kMilliseconds.hpp"

namespace klib
{
	namespace kCalendar
	{
		namespace secret::helper
		{
			class iCalendarInfoSource;
		}
		
		class Time
		{
		public:
			enum TimeComponent : uint8_t
			{
				HOURS,
				MINS,
				SECS,
				MILLIS,
				END_MARKER
			};
			
		public:
			explicit Time(CalendarInfoSourceType sourceType);
			Time(const Hour::CycleType cycle, const Hour h, const Minute m, const Second s,
				const Millisecond ms = Millisecond(0));

			USE_RESULT std::uint16_t GetComponent(const TimeComponent timeComponent) const;
			USE_RESULT std::string ToString(const TimeComponent accuracy = MILLIS) const;
			USE_RESULT std::string ToString(const std::string_view& format) const;

			USE_RESULT const Hour& GetHours() const;
			USE_RESULT const Minute& GetMinutes() const;
			USE_RESULT const Second& GetSeconds() const;
			USE_RESULT const Millisecond& GetMilliseconds() const;
			
		private:
			void CheckTime() const;

			explicit Time(const secret::helper::iCalendarInfoSource& timeSource);
			
		private:
			Hour hours;
			Minute minutes;
			Second seconds;
			Millisecond milliseconds;
		};
	}

#ifdef KLIB_SHORT_NAMESPACE
		using namespace kCalendar;
#endif
}
