#pragma once

#include <cstdint>
#include <string>

#include "../../HelperMacros.hpp"


struct _SYSTEMTIME;

namespace klib
{
	namespace kCalendar
	{
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
