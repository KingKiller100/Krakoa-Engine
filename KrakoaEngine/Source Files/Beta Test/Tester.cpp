#include "Tester.h"

#include "../Utility/Calendar/kCalendar.h"
#include "../Utility/Debug Helper/kDebugger.h"
#include "../Utility/File System/kFileSystem.h"
#include "../Utility/Clock/kClock.h"
#include "../Utility/Logging/kLogging.h"

#include "../Utility/Maths/Vectors/Vector2.h"

#include <iostream>

namespace krakoa::kTest
{
	using namespace util;
	
	void Print_Test()
	{
		std::cout << "Welcome to the Krakoa Engine!" << std::endl;
	}

	void Debugger_Test()
	{
		kDebugger::CheckRemoteDebuggerAttached("waitforremotedebugger");
		//kDebugger::BreakPointHere(); // Works Great!
	}

	void Clock_Test()
	{
		kTime::Clock<float>::Start();
		kTime::Clock<double>::GetDeltaTime();
		kTime::Clock<long double>::GetSystemLifeTime();
	}

	void FileSystem_Test()
	{
		kFileSystem::CheckFileExists("C:\\Users\\kweku\\Documents\\Krakoa Engine\\bin\\x64\\Debug\\Hooper2\\waitforremotedebugger");
		const auto cwd = kFileSystem::GetCurrentWorkingDirectory();
		kFileSystem::CreateNewDirectories(cwd + "Create Directories Test\\Success1\\Success2\\");
		kFileSystem::CreateNewDirectory((cwd + "Create Directory Test\\").c_str());
		kFileSystem::DeleteDirectory((cwd + "Create Directories Test\\Success1\\Success2").c_str());
		kFileSystem::OutputToFile((cwd + "Create Directory Test\\Test.txt").c_str(), "Success\n");
		kFileSystem::ParseFileData((cwd + "Create Directory Test\\Test.txt").c_str());
	}

	void Calendar_Test()
	{
		const auto dateTimeTest = kCalendar::GetSystemDateAndTime();
		const auto time = kCalendar::GetComponentOfTime(kCalendar::TimeComponent::millisec);
		auto calendarTest = kCalendar::GetDateInNumericalFormat();
		calendarTest = kCalendar::GetDateInTextFormat();
		calendarTest = kCalendar::GetTimeText();
		calendarTest = kCalendar::GetDayOfTheWeek(3);
		calendarTest = kCalendar::GetMonth(3);
	}

	void Logger_Test()
	{
		kLogs::Logging logger = kLogs::Logging();
		logger.InitializeLogging();
		logger.AddEntry("NORMAL!", kLogs::LogLevel::NORM);
		logger.AddEntryBanner("BANNER!", "TEST");
		logger.AddEntry("INFORMATIVE!", kLogs::LogLevel::INFO);
		logger.AddEntry("ERROR!", kLogs::LogLevel::ERRR);
		logger.AddEntry("FATAL!", kLogs::LogLevel::FATL);
		logger.GetLastLoggedEntry();
		logger.ErasePreviousEntries(2);
		logger.Output();
		logger.AddEntry("Done", kLogs::LogLevel::WARN);	
		logger.AppendLogFile();
	}

	void Math_Vector_Test()
	{
		using namespace kMaths;
		auto test = Vector2<double>(5, -10);
		test.ToPositives();
		test.ReverseVector();
		test.X();
		test.Y(19);
		test.Truncate(30);
	}
}
