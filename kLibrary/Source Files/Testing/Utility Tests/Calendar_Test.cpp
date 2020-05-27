#include "pch.hpp"
#include "Calendar_Test.hpp"

#include "../../Utility/Calendar/kCalendar.hpp"


#ifdef TESTING_ENABLED
namespace kTest::utility
{

	CalendarTester::CalendarTester()
		: Tester("Calendar Test")
	{}

	CalendarTester::~CalendarTester()
		= default;

	using namespace klib::kCalendar;

	template<typename T, typename Arg, typename  ...Args>
	std::basic_string<T> MakeString(const T* format, Arg arg, Args ...args)
	{
		T* buffer = nullptr;

		if _CONSTEXPR_IF(std::is_same_v<T, char>)
		{
			const auto length = 1 + _snprintf_l(nullptr, 0, format, nullptr, arg, args...);
			if (length <= -1) throw std::exception();
			buffer = new T[length]{};
			sprintf_s(buffer, length, format, arg, args...);
		}
		else if _CONSTEXPR_IF(std::is_same_v<T, wchar_t>)
		{
			const auto length = 1 + _snwprintf_l(nullptr, 0, format, nullptr, arg, args...);
			if (length <= -1) throw std::exception();
			buffer = new T[length]{};
			swprintf_s(buffer, length, format, arg, args...);
		}

		std::basic_string<T> str = buffer;
		delete[] buffer;
		return str;
	}

	void CalendarTester::Test()
	{
		VERIFY_MULTI_INIT();
		VERIFY_MULTI(MonthTest);
		VERIFY_MULTI(CreateTimeTest);
		VERIFY_MULTI(GetTimeTextTest);
		VERIFY_MULTI(DayOfTheWeekTest);
		VERIFY_MULTI(GetTimeComponentTest);
		VERIFY_MULTI(GetDateInTextFormatTest);
		VERIFY_MULTI(GetDateInNumericalFormatTest);
		VERIFY_MULTI_END();
	}

	bool CalendarTester::MonthTest()
	{
		{
			constexpr auto result = GetMonth(2);
			VERIFY_COMPILE_TIME(result.compare("March") == 0);
		}

		{
			constexpr auto result = GetMonth(5);
			VERIFY_COMPILE_TIME(result.compare("June") == 0);
		}

		{
			constexpr auto result = GetMonth(11);
			VERIFY_COMPILE_TIME(result.compare("December") == 0);
		}

		{
			try
			{
				const auto result = GetMonth(12);
			}
			catch (const klib::kDebug::CalendarError & e)
			{
				constexpr std::string_view error = "Value entered does not index to a month of the year";
				VERIFY(error.compare(e.what()) == 0);
			}
		}

		return success;
	}

	bool CalendarTester::GetTimeTextTest()
	{
		{
			SYSTEMTIME localTime;
			::GetLocalTime(&localTime);
			const auto result = GetTimeText();
			const auto expected = MakeString("%02d:%02d:%02d:%03d", 
				localTime.wHour, 
				localTime.wMinute, 
				localTime.wSecond, 
				localTime.wMilliseconds);
			VERIFY(result == expected);
		}
		
		{
			SYSTEMTIME localTime;
			::GetLocalTime(&localTime);
			const auto result = wGetTimeText();
			const auto expected = MakeString(L"%02d:%02d:%02d:%03d", 
				localTime.wHour, 
				localTime.wMinute, 
				localTime.wSecond, 
				localTime.wMilliseconds);
			VERIFY(result == expected);
		}
		
		{
			SYSTEMTIME systemTime;
			::GetSystemTime(&systemTime);
			const auto result = GetTimeText(GetSystemDateAndTime());
			const auto expected = MakeString("%02d:%02d:%02d:%03d", 
				systemTime.wHour, 
				systemTime.wMinute, 
				systemTime.wSecond, 
				systemTime.wMilliseconds);
			VERIFY(result == expected);
		}
		
		{
			SYSTEMTIME systemTime;
			::GetSystemTime(&systemTime);
			const auto result = wGetTimeText(GetSystemDateAndTime());
			const auto expected = MakeString(L"%02d:%02d:%02d:%03d", 
				systemTime.wHour, 
				systemTime.wMinute, 
				systemTime.wSecond, 
				systemTime.wMilliseconds);
			VERIFY(result == expected);
		}

		
		return success;
	}

	bool CalendarTester::DayOfTheWeekTest()
	{
		{
			constexpr auto result = GetDayOfTheWeek(2);
			VERIFY_COMPILE_TIME(result.compare("Tuesday") == 0);
		}

		{
			constexpr auto result = GetDayOfTheWeek(5);
			VERIFY_COMPILE_TIME(result.compare("Friday") == 0);
		}

		{
			constexpr auto result = GetDayOfTheWeek(0);
			VERIFY_COMPILE_TIME(result.compare("Sunday") == 0);
		}

		{
			try
			{
				const auto result = GetDayOfTheWeek(7);
			}
			catch (const klib::kDebug::CalendarError & e)
			{
				constexpr std::string_view error = "Value entered does not index to a day of the week";
				VERIFY(error.compare(e.what()) == 0);
			}
		}

		return success;
	}

	bool CalendarTester::GetTimeComponentTest()
	{
		{
			SYSTEMTIME localTime;
			::GetLocalTime(&localTime);

			const auto hour = GetComponentOfTime(TimeComponent::HOURS);
			VERIFY(hour == localTime.wHour);
		}

		{
			SYSTEMTIME localTime;
			::GetLocalTime(&localTime);

			const auto mins = GetComponentOfTime(TimeComponent::MINS);
			VERIFY(mins == localTime.wMinute);
		}

		{
			SYSTEMTIME localTime;
			::GetLocalTime(&localTime);

			const auto secs = GetComponentOfTime(TimeComponent::SECS);
			VERIFY(secs == localTime.wSecond);
		}

		{
			SYSTEMTIME localTime;
			::GetLocalTime(&localTime);

			const auto millis = GetComponentOfTime(TimeComponent::MILLIS);
			VERIFY(millis == localTime.wMilliseconds);
		}


		return success;
	}

	template<typename T >
	constexpr auto DateTextFunc(const SYSTEMTIME dateTime, DateFormat format = klib::kCalendar::DateFormat::FULL) -> decltype(auto)
	{
		constexpr std::array<const char*, 12> months = {
			"January", "February", "March",
			"April", "May", "June",
			"July", "August", "September",
			"October", "November", "December"
		};

		constexpr std::array<const wchar_t*, 12> wMonths = {
			L"January", L"February", L"March",
			L"April", L"May", L"June",
			L"July", L"August", L"September",
			L"October", L"November", L"December"
		};

		constexpr  std::array<const char*, 7> days = {
			"Sunday", "Monday", "Tuesday", "Wednesday",
		"Thursday", "Friday", "Saturday"
		};

		constexpr  std::array<const wchar_t*, 7> wDays = {
			L"Sunday", L"Monday", L"Tuesday", L"Wednesday",
		L"Thursday", L"Friday", L"Saturday"
		};

		std::basic_string<T> day;
		std::basic_string<T> month;

		std::basic_string<T> dateStr;

		if _CONSTEXPR_IF(std::is_same_v<T, char>)
		{
			day = days[dateTime.wDayOfWeek];
			if (format != DateFormat::FULL) day = day.substr(0, 3);
			month = months[dateTime.wMonth - 1];

			dateStr = MakeString<T>("%s %d %s %04d",
				day.data(),
				dateTime.wDay,
				month.data(),
				dateTime.wYear);
		}
		else if _CONSTEXPR_IF(std::is_same_v<T, wchar_t>)
		{
			day = wDays[dateTime.wDayOfWeek];
			if (format != DateFormat::FULL) day = day.substr(0, 3);
			month = wMonths[dateTime.wMonth - 1];

			dateStr = MakeString<T>(L"%s %d %s %04d",
				day.data(),
				dateTime.wDay,
				month.data(),
				dateTime.wYear);
		}

		return dateStr;
	};



	bool CalendarTester::GetDateInTextFormatTest()
	{
		{
			SYSTEMTIME localTime;
			::GetLocalTime(&localTime);
			const auto result = GetDateInTextFormat();
			const auto expected = DateTextFunc<char>(localTime);
			VERIFY(result == expected);
		}

		{
			SYSTEMTIME localTime;
			::GetLocalTime(&localTime);
			const auto result = GetDateInTextFormat(DateFormat::SHORT);
			const auto expected = DateTextFunc<char>(localTime, DateFormat::SHORT);
			VERIFY(result == expected);
		}

		{
			SYSTEMTIME systemTime;
			::GetSystemTime(&systemTime);
			const auto result = GetDateInTextFormat(DateFormat::SHORT, GetSystemDateAndTime());
			const auto expected = DateTextFunc<char>(systemTime, DateFormat::SHORT);
			VERIFY(result == expected);
		}

		{
			SYSTEMTIME localTime;
			::GetLocalTime(&localTime);
			const auto result = wGetDateInTextFormat();
			const auto expected = DateTextFunc<wchar_t>(localTime);
			VERIFY(result == expected);
		}

		{
			SYSTEMTIME localTime;
			::GetLocalTime(&localTime);
			const auto result = wGetDateInTextFormat(DateFormat::SHORT);
			const auto expected = DateTextFunc<wchar_t>(localTime, DateFormat::SHORT);
			VERIFY(result == expected);
		}

		{
			SYSTEMTIME systemTime;
			::GetSystemTime(&systemTime);
			const auto result = wGetDateInTextFormat(DateFormat::SHORT, GetSystemDateAndTime());
			const auto expected = DateTextFunc<wchar_t>(systemTime, DateFormat::SHORT);
			VERIFY(result == expected);
		}


		return success;
	}

	bool CalendarTester::GetDateInNumericalFormatTest()
	{
		{
			SYSTEMTIME localTime;
			::GetLocalTime(&localTime);
			const auto result = wGetDateInNumericalFormat();
			const auto expected = klib::kFormat::ToString(L"%02d/%02d/%02d", localTime.wDay, localTime.wMonth, localTime.wYear);
			VERIFY(result == expected)
		}

		{
			SYSTEMTIME localTime;
			::GetLocalTime(&localTime);
			const auto result = GetDateInNumericalFormat(DateSeparator::DASH);
			const auto expected = klib::kFormat::ToString("%02d-%02d-%02d", localTime.wDay, localTime.wMonth, localTime.wYear);
			VERIFY(result == expected)
		}

		{
			SYSTEMTIME systemTime;
			::GetSystemTime(&systemTime);
			const auto result = GetDateInNumericalFormat(DateSeparator::SLASH, GetSystemDateAndTime());
			const auto expected = klib::kFormat::ToString("%02d/%02d/%02d", systemTime.wDay, systemTime.wMonth, systemTime.wYear);
			VERIFY(result == expected)
		}

		{
			SYSTEMTIME systemTime;
			::GetSystemTime(&systemTime);
			const auto result = wGetDateInNumericalFormat(DateSeparator::DASH, GetSystemDateAndTime());
			const auto expected = klib::kFormat::ToString(L"%02d-%02d-%02d", systemTime.wDay, systemTime.wMonth, systemTime.wYear);
			VERIFY(result == expected)
		}


		return success;
	}

	bool CalendarTester::CreateTimeTest()
	{
		{
			const auto result = CreateTime(12, 30, 25);
			constexpr const auto* const expected = "12:30:25";
			VERIFY(result == expected);
		}
		
		{
			const auto result = wCreateTime(16, 18, 0, 567);
			constexpr const auto* const expected = L"16:18:00:567";
			VERIFY(result == expected);
		}
		
		{
			const auto result = wCreateTime(6, 2, 8, 24);
			constexpr const auto* const expected = L"06:02:08:024";
			VERIFY(result == expected);
		}
		
		{
			const auto result = CreateTime(6, 20);
			constexpr const auto* const expected = "06:20";
			VERIFY(result == expected);
		}
		
		{
			const auto result = CreateTime(24, 60, 60);

			constexpr const auto* const bad = "24:60:60";
			VERIFY(result != bad);
			constexpr const auto* const expected = "23:59:59";
			VERIFY(result == expected);
		}

		
		return success;
	}
}
#endif

