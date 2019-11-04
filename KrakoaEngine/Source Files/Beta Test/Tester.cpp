#include "Tester.h"

#include "../Utility/Calendar/kCalendar.h"
#include "../Utility/Debug Helper/kDebugger.h"
#include "../Utility/File System/kFileSystem.h"
#include "../Utility/Clock/kClock.h"
#include "../Utility/Logging/kLogging.h"

#include "../Utility/Maths/Vectors/Vector2.h"
#include "../Utility/Maths/Vectors/Vector3.h"
#include "../Utility/Maths/Vectors/Vector4.h"

#include "../Utility/String View/string_view.h"

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
		logger.ChangeOutputDirectory(kFileSystem::GetCurrentWorkingDirectory() + "Change Dir\\");
		logger.ChangeFilename("DiffFileName");
		logger.AddEntry("NORMAL!", kLogs::LogLevel::NORM);
		logger.AddEntryBanner("BANNER!", "TEST");
		logger.AddEntry("INFORMATIVE!", kLogs::LogLevel::INFO);
		logger.AddEntry("ERROR!", kLogs::LogLevel::ERRR);
		logger.AddEntry("FATAL!", kLogs::LogLevel::FATL);
		logger.GetLastLoggedEntry();
		logger.ErasePreviousEntries(2);
		logger.AppendLogFile();
		logger.AddEntry("Done", kLogs::LogLevel::WARN);	
		logger.Output();
	}

	void Math_Vector2_Test()
	{
		using namespace kMaths;
		auto v2 = Vector2<double>(5, -10);
		v2.ToPositives();
		v2.ReverseVector();
		v2.X();
		v2.Y();
		v2 *= 8;
		v2.Truncate(30);
		const auto temp = v2.Magnitude();
		const auto temp1 = v2.Perpendicular();
		v2.Distance(temp1);
		const auto temp2 = -v2;
		const auto temp3 = v2.Magnitude();
		auto res = v2 != temp2;
		v2.Zero();
	}

	void Math_Vector3_Test()
	{
		using namespace kMaths;
		 auto v3 = Vector3f();
		const Vector3f tempV3 = Vector3f(v3.Z());

		CrossProduct(v3, tempV3);
		//v3.CrossProduct(Vector3f());
	}

	void Math_Vector4_Test()
	{
		using namespace kMaths;
		const auto v4 = Vector4s(5,7, 3, 1);
		
		auto p = v4.W();
	}

	void String_View_Test()
	{
		using namespace kString_View;
		auto t = "hooper2";
		StringView s = t;
		wStringView s2(L"string literal");
		u32StringView s3 = U"std string";
		s.RemovePrefix(3);
		s.RemoveSuffix(1);
		auto pos = s.FirstInstanceOf('e');
		auto pos1 = s.FirstInstanceOf('g', 1, 2);
		auto pos2 = s.FirstInstanceOf("er");
		auto k = s2.Back();
		k = s2.Front();
		wStringView s4(L"Empty");
		s2.Swap(s4);
		auto ptr = s2.Data();
		s2.Clear();
		auto b = s2.Empty();
		auto l = s3.Length();
		auto v = s3.Substr(5);
		auto temp = s.Substr(s.Length());
		auto h = s.IsEqual(temp);
	}
}
