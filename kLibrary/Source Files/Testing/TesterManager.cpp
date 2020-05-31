#include "pch.hpp"
#include "TesterManager.hpp"

#include "Tester.hpp"

// Maths Tests
#include "../../Tests/Maths Tests/Random_Test.hpp"
#include "../../Tests/Maths Tests/Matrix_Test.hpp"
#include "../../Tests/Maths Tests/Vectors_Test.hpp"
#include "../../Tests/Maths Tests/Fraction_Test.hpp"
#include "../../Tests/Maths Tests/Algorithm_Test.hpp"
#include "../../Tests/Maths Tests/Quaternion_Test.hpp"

// Utility
#include "../../Tests/Utility Tests/Timer_Test.hpp"
#include "../../Tests/Utility Tests/Logging_Test.hpp"
#include "../../Tests/Utility Tests/Calendar_Test.hpp"
#include "../../Tests/Utility Tests/DebugHelp_Test.hpp"
#include "../../Tests/Utility Tests/FileSystem_Test.hpp"
#include "../../Tests/Utility Tests/StringView_Test.hpp"
#include "../../Tests/Utility Tests/UTFConverter_Test.hpp"
#include "../../Tests/Utility Tests/StringManipulation_Test.hpp"
#include "../../Tests/Utility Tests/FormatToString_Test.hpp"

// Templates
#include "../../Tests/Templates Tests/BytesStorage_Test.hpp"

// Speed Testing
#include "../../Tests/Performance Tests/PerformanceTestManager.hpp"

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
		kFileSystem::RemoveFile(path.c_str());
	}

	void TesterManager::InitializeMathsTests()
	{
		Add(new maths::FractionTester());
		Add(new maths::AlgorithmsTester());
		Add(new maths::VectorsTester());
		Add(new maths::MatricesTester());
		Add(new maths::QuaternionsTester());
		Add(new maths::RandomTester());
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

	void TesterManager::InitializeTemplateTests()
	{
		Add(new templates::BytesStorageTester());
	}

	void TesterManager::RunPerformanceTests() const
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

	void TesterManager::Run(Tester& test)
	{
		using namespace klib::kFormat;
		
		const klib::kTime::HighAccuracyTimer runTimeTimer("Test Run Time");

		std::cout << "Now running: " << test.GetName() << "";

		const auto result = test.Run();

		const auto runtimeStr = ToString(
			"| Runtime: %.fus (microseconds)",
			runTimeTimer.GetLifeTime<klib::kTime::units::Micros>());
		std::cout << " " << runtimeStr << "\n";

		if (!result)
			success = false;

		const auto resultTest = result
			? ToString("Success: Test Name: {0} {1}\n\n", 
				test.GetName(),
				runtimeStr) // Success Case
			: ToString("Failure: Test Name: {0} {1}\n{2}",
				test.GetName(), 
				runtimeStr, 
				test.GetFailureData()); // Fail Case

		klib::kFileSystem::OutputToFile(path.c_str(), resultTest.c_str());
	}

	void TesterManager::RunAll()
	{
		klib::kTime::HighAccuracyTimer totalRunTimeTimer("Total Test Run Time");

		for (const auto& test : testsUSet)
		{
			Run(*test);
		}

		const auto finalTime = totalRunTimeTimer.GetDeltaTime<klib::kTime::units::Secs>();
		const auto secs = CAST(unsigned, finalTime);
		const auto remainder = finalTime - secs;
		const unsigned millis = CAST(unsigned, 1000.0 * remainder);

		const auto finalTimeStr = klib::kFormat::ToString("Total Runtime: {0}s  {1}ms", secs, millis);
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
