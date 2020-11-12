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
		const auto year = years
			- static_cast<double>(noOfLeaps) / Day::DaysInYear
			+ static_cast<double>(remainingDays) / Day::DaysInYear;
		return year;
	}

	USE_RESULT constexpr double YearsFromMonths(const size_t months)
	{
		const auto year = static_cast<double>(months) / Month::MonthsInYear;
		return year;
	}

	USE_RESULT constexpr double MonthsFromDays(const size_t days)
	{
		constexpr auto divider = 30.4167;
		const auto months = days / divider;
		return months;
	}

	// Time
	template<typename DestTimeT, typename SourceTimeT, typename = std::enable_if_t<
		std::_Is_specialization_v<typename DestTimeT::Self_t, TimeComponentBase>
		&& std::_Is_specialization_v<typename SourceTimeT::Self_t, TimeComponentBase>
		>>
		USE_RESULT constexpr DestTimeT TimeConverter(const SourceTimeT& source)
	{
		using namespace std::chrono;
		using DestUnderlyingT = typename DestTimeT::Underlying_t;

		const auto val =
			duration_cast<DestUnderlyingT>(source.GetUnderlying());
		const DestTimeT dest = DestTimeT(val.count());
		return dest;
	}
}
