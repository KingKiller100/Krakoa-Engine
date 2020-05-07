#include "pch.hpp"
#include "TesterManager.hpp"

#include "Tester.hpp"

// Maths Tests
#include "Maths Tests/Matrix_Test.hpp"
#include "Maths Tests/Vectors_Test.hpp"
#include "Maths Tests/Fraction_Test.hpp"
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

// Speed Testing
#include "Performance Tests/PerformanceTestManager.hpp"

// File System to output test results
#include "../Utility/File System/kFileSystem.hpp"

// Times the length of the test
#include "../Utility/Timer/kTimer.hpp"

#include <iostream>

#ifdef TESTING_ENABLED
namespace kTest
{
	TesterManager::TesterManager(Token&)
		: success(true)
	{	}

	TesterManager::~TesterManager()
		= default;

	void TesterManager::Shutdown()
	{
		ClearAllTests();
	}

	void TesterManager::Initialize()
	{
		using namespace klib;
		path = kFileSystem::GetExeDirectory<char>() + "Test Results\\";
		const auto isMade = kFileSystem::CreateNewDirectory(path.c_str());

		if (!kFileSystem::CheckDirectoryExists(path))
		{
			throw std::runtime_error("Test Results directory could not be created/found. Please check why!");
		}

		path += "Results.txt";
		klib::kFileSystem::RemoveFile(path.c_str());
	}

	void TesterManager::InitializeMathsTests()
	{
		Add(new maths::FractionTester());
		Add(new maths::AlgorithmsTester());
		Add(new maths::VectorsTester());
		Add(new maths::MatricesTester());
		Add(new maths::QuaternionsTester());
	}

	void TesterManager::InitializeUtilityTests()
	{
		Add(new utility::UTFConverterTester());
		Add(new utility::StringManipulationTester());
		Add(new utility::FormatToStringTester());
		Add(new utility::CalendarTester());
		Add(new utility::FileSystemTester());
		Add(new utility::DebugHelpTester());
		Add(new utility::LoggingTester());
		Add(new utility::StringViewTester());
		Add(new utility::TimerTester());
	}

	void TesterManager::RunPerformanceTests()
	{
		if (success)
		{
			std::cout << std::endl;
			auto& test = performance::PerformanceTestManager::Get();
			std::cout << "Now Testing: " << test.GetName() << " ";
			test.Run();
		}
	}

	void TesterManager::Add(Tester* test)
	{
		testsUSet.insert(std::unique_ptr<Tester>(std::move(test)));
	}

	void TesterManager::Run(Tester* test)
	{
		klib::kTime::HighAccuracyTimer runTimeTimer("Test Run Time");

		std::cout << "Now running: " << test->GetName() << "";

		const auto result = test->Run();

		const auto runtimeStr = klib::kFormat::ToString("| Runtime: %.fus (Microseconds)", runTimeTimer.GetLifeTime<klib::kTime::units::Micros>());
		std::cout << " " << runtimeStr << "\n";

		if (!result)
			success = false;

		const auto resultTest = result
			? klib::kFormat::ToString("Success: Test Name: {0} {1}\n\n", test->GetName(), runtimeStr) // Success Case
			: klib::kFormat::ToString("Failure: Test Name: {0} (1}\n{2}", test->GetName(), runtimeStr, test->GetFailureData()); // Fail Case

		klib::kFileSystem::OutputToFile(path.c_str(), resultTest.c_str());
	}

	void TesterManager::RunAll()
	{
		klib::kTime::HighAccuracyTimer totalRunTimeTimer("Total Test Run Time");

		for (const auto& test : testsUSet)
		{
			Run(test.get());
		}

		const auto finalTime = totalRunTimeTimer.GetDeltaTime<klib::kTime::units::Secs>();
		const auto secs = CAST(unsigned, finalTime);
		const auto remainder = finalTime - secs;
		const unsigned millisecs = CAST(unsigned, 1000.0 * remainder);


		const auto finalTimeStr = klib::kFormat::ToString("Total Runtime: {0}s  {1}ms", secs, millisecs);
		klib::kFileSystem::OutputToFile(path.c_str(), finalTimeStr.c_str());

		std::cout << "\n" << finalTimeStr << "\n";

		std::cout << "\nTests have concluded. Please find results in the following path:\n" << path << std::endl;
		std::cin.get();
	}

	void TesterManager::ClearAllTests()
	{
		testsUSet.clear();
	}

	TesterManager& TesterManager::Get()
	{
		static Token t;
		static TesterManager instance(t);
		return instance;
	}
}
#endif // TESTING_ENABLED
