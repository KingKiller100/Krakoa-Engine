#include "Tester.h"

#include "../Utility/Calendar/kCalendar.h"
#include "../Utility/Debug Helper/kDebugger.h"
#include "../Utility/File System/kFileSystem.h"
#include "../Utility/Clock/kClock.h"

#include <iostream>

namespace krakoa::kTest
{
	void Print_Test()
	{
		std::cout << "Welcome to the Krakoa Engine!" << std::endl;
	}

	void Debugger_Test()
	{
		kDebugger::CheckRemoteDebuggerAttached("waitforremotedebugger");
		//kDebugger::BreakPointHere(); // Works Great!
	}

	void Timer_Test()
	{
		kTime::Clock<>::Start();
		kTime::Clock<>::GetDeltaTime();
		kTime::Clock<>::GetSystemLifeTime();
	}

	void FileSystem_Test()
	{
		kFileSystem::CheckFileExists("C:\\Users\\kweku\\Documents\\Krakoa Engine\\bin\\x64\\Debug\\Hooper2\\waitforremotedebugger");
		kFileSystem::CheckFileExists("null");
	}

	void Calendar_Test()
	{
		const auto dateTimeTest = kCalendar::GetSystemDateAndTime();
		const auto time = kCalendar::GetComponentOfTime(kCalendar::TimeComponent::millisec);
		auto calendarTest = kCalendar::GetDateInNumericalFormat();
		calendarTest = kCalendar::GetDateInTextFormat();
		calendarTest = kCalendar::GetTimeStr();
		calendarTest = kCalendar::GetDayOfTheWeek(1);
		calendarTest = kCalendar::GetMonth(1);
	}
}
