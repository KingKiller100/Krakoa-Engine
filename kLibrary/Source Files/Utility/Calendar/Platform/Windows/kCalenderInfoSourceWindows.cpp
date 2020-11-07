#include "pch.hpp"
#include "kCalenderInfoSourceWindows.hpp"

#ifdef _WIN64

namespace klib::kCalendar::secret::helper::windows
{
	SYSTEMTIME& GetLocalDateAndTime() noexcept
	{
		static SYSTEMTIME kCalendar_Local_DateTime;
		GetLocalTime(&kCalendar_Local_DateTime);
		return kCalendar_Local_DateTime;
	}

	SYSTEMTIME& GetSystemDateAndTime() noexcept
	{
		static SYSTEMTIME kCalendar_System_DateTime;
		GetSystemTime(&kCalendar_System_DateTime);
		return kCalendar_System_DateTime;
	}

	SYSTEMTIME& GetDateAndTime(CalendarInfoSourceType type)
	{
		return type == CalendarInfoSourceType::LOCAL
			? GetLocalDateAndTime()
			: GetSystemDateAndTime();
	}
}

#endif
