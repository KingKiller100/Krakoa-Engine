﻿#pragma once
#include "../../kCalenderInfoSourceType.hpp"
#include "../../kiCalendarInfoSource.hpp"

#if defined(_WIN64) || defined(_WIN32)

#	include <Windows.h>

namespace klib::kCalendar::windows
{
	class CalendarInfoSourceWindows : public iCalendarInfoSource
	{
	public:
		CalendarInfoSourceWindows();
		~CalendarInfoSourceWindows();
		
		void Refresh(CalendarInfoSourceType type) override;
		USE_RESULT std::uint16_t GetDay() const override;
		USE_RESULT std::uint16_t GetDayOfTheWeekIndex() const override;
		USE_RESULT std::uint16_t GetMonth() const override;
		USE_RESULT std::uint16_t GetYear() const override;
		USE_RESULT std::uint16_t GetHour() const override;
		USE_RESULT std::uint16_t GetMinute() const override;
		USE_RESULT std::uint16_t GetSecond() const override;
		USE_RESULT std::uint16_t GetMillisecond() const override;

	private:
		void RefreshLocal() noexcept;
		void RefreshSystem() noexcept;
		
	private:
		SYSTEMTIME dateTime;
	};

}

#endif