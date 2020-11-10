#pragma once
#include <chrono>

#include "Date/kDay.hpp"
#include "Date/kMonth.hpp"
#include "Date/kYear.hpp"

namespace klib::kCalendar
{
	// Date
	USE_RESULT constexpr double YearsFromDays(const size_t days)
	{
		const auto noOfLeaps = days / Year::LeapYearFrequency / Day::DaysInYear;
		const auto years = days / Day::DaysInYear;
		const auto remainingDays = days % Day::DaysInYear;
		const auto year = (years - noOfLeaps / Day::DaysInYear)
			+ (static_cast<double>(remainingDays) / Day::DaysInYear);
		return year;
	}

	USE_RESULT constexpr double YearsFromMonths(const size_t months)
	{
		const auto year = static_cast<double>(months) / Month::MonthsInYear;
		return year;
	}

	USE_RESULT constexpr double MonthsFromDays(const size_t days)
	{
		const auto months = YearsFromDays(days) * Month::MonthsInYear;
		return months;
	}

	// Time
	template<typename DestTime, typename SourceTime>
	USE_RESULT constexpr DestTime TimeConverter(const TimeComponentBase<SourceTime>& source)
	{
		using namespace std::chrono;
		const auto val =
			duration_cast<typename DestTime::UnderlyingT>
			(source.GetUnderlying());
		const DestTime dest = DestTime(val.count());
		return dest;
	}
}
