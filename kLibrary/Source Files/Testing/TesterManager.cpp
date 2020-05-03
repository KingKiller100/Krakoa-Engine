#include "pch.hpp"
#include "TesterManager.hpp"

#include "Tester.hpp"

// Maths Tests
#include "Maths Tests/Matrix_Test.hpp"
#include "Maths Tests/Vectors_Test.hpp"
#include "Maths Tests/Algorithm_Test.hpp"
#include "Maths Tests/Quaternion_Test.hpp"

// Utility
#include "Utility Tests/Timer_Test.hpp"
#include "Utility Tests/Logging_Test.hpp"
#include "Utility Tests/Calendar_Test.hpp"
#include "Utility Tests/DebugHelp_Test.hpp"
#include "Utility Tests/FileSystem_Test.hpp"
#include "Utility Tests/StringView_Test.hpp"
#include "Utility Tests/UTFConverter_Test.hpp"
#include "Utility Tests/StringManipulation_Test.hpp"
#include "Utility Tests/FormatToString_Test.hpp"

// File System to output test results
#include "../Utility/File System/kFileSystem.hpp"

// Times the length of the test
#include "../Utility/Timer/kTimer.hpp"

#include <unordered_set>

#ifdef TESTING_ENABLED
namespace kTest
{
	std::unordered_set< std::unique_ptr<Tester> > kTests_TestsUSet;
	std::string kTest_TestResultFilePath;
	
	TesterManager::TesterManager()
	{	}

	TesterManager::~TesterManager()
		= default;

	void TesterManager::Shutdown()
	{
		ClearAllTests();
	}

	void TesterManager::Initialize()
	{
		kTest_TestResultFilePath = klib::kFileSystem::GetExeDirectory<char>() + "Test Results\\";
		const auto isMade = klib::kFileSystem::CreateNewDirectory(kTest_TestResultFilePath.c_str());
		
		kTest_TestResultFilePath += "Results.txt";
		klib::kFileSystem::RemoveFile(kTest_TestResultFilePath.c_str());
	}

	void TesterManager::InitializeMathsTests()
	{
		Add(new maths::AlgorithmsTester());
		Add(new maths::VectorsTester());
		Add(new maths::MatricesTester());
		Add(new maths::QuaternionsTester());
	}

	void TesterManager::InitializeUtilityTests()
	{		
		Add(new utility::StringManipulationTester());
		Add(new utility::FormatToStringTester());
		Add(new utility::CalendarTester());
		Add(new utility::FileSystemTester());
		Add(new utility::DebugHelpTester());
		Add(new utility::LoggingTester());
		Add(new utility::UTFConverterTester());
		Add(new utility::StringViewTester());
		Add(new utility::TimerTester());
	}

	void TesterManager::Add(Tester* test)
	{
		kTests_TestsUSet.insert(std::unique_ptr<Tester>(std::move(test)));
	}

	void TesterManager::RunAll()
	{
		klib::kTime::HighAccuracyTimer totalRunTimeTimer("Total Test Run Time");
		for (const auto& test : kTests_TestsUSet)
		{
			const auto resultTest = test->Run() 
				? klib::kFormat::ToString("Success: Test Name: {0}\n\n", test->GetName()) // Success Case
				: klib::kFormat::ToString("Failure: Test Name: {0}\n{1}", test->GetName(), test->GetFailureData()); // Fail Case

			klib::kFileSystem::OutputToFile(kTest_TestResultFilePath.c_str(), resultTest.c_str());
		}

		const auto finalTime = totalRunTimeTimer.GetDeltaTime<klib::kTime::kUnits::Mins>();
		const auto mins = CAST(unsigned, finalTime);
		const auto remainder = finalTime - mins;
		const unsigned secs = CAST(unsigned, 60.0 * remainder);

		const auto finalTimeStr = klib::kFormat::ToString("Total Runtime: {0}m  {1}s", mins, secs);
		klib::kFileSystem::OutputToFile(kTest_TestResultFilePath.c_str(), finalTimeStr.c_str());
	}

	void TesterManager::ClearAllTests()
	{
		kTests_TestsUSet.clear();
	}
	
	TesterManager& TesterManager::Get()
	{
		static TesterManager pInstance;
		return pInstance;
	}
}
#endif // TESTING_ENABLED
