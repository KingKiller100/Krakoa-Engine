#pragma once
#include "../../kCalenderInfoSourceType.hpp"

#ifdef _WIN64

#	include <Windows.h>

namespace klib::kCalendar::secret::helper::windows
{
	SYSTEMTIME& GetLocalDateAndTime() noexcept;
	SYSTEMTIME& GetSystemDateAndTime() noexcept;
	SYSTEMTIME& GetDateAndTime(CalendarInfoSourceType type);
}

#endif