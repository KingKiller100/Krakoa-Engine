#include <pch.hpp>
#include <Testing/TesterManager.hpp>

#include <Testing/Tester.hpp>

// Maths Tests
#include <Testing/Maths Tests/Vectors_Test.hpp>

// Utility Tests
#include <Testing/Utility Tests/Timer_Test.hpp>
#include <Testing/Utility Tests/Logging_Test.hpp>
#include <Testing/Utility Tests/Calendar_Test.hpp>
#include <Testing/Utility Tests/DebugHelp_Test.hpp>
#include <Testing/Utility Tests/FileSystem_Test.hpp>
#include <Testing/Utility Tests/StringView_Test.hpp>
#include <Testing/Utility Tests/FormatToString_Test.hpp>

// File System to output test results
#include <Utility/File System/kFileSystem.hpp>

#include <unordered_set>

namespace kTest
{
	std::unordered_set< std::unique_ptr<Tester> > kTests_TestsUSet;
	std::string kTest_TestResultFilePath;
	
	TesterManager::TesterManager(Token)
	{
	}

	TesterManager::~TesterManager()
		= default;

	void TesterManager::Shutdown()
	{
	}

	void TesterManager::Initialize()
	{
#ifdef TESTING_ENABLED
		kTest_TestResultFilePath = util::kFileSystem::GetCurrentWorkingDirectory<char>() + "Test Results\\";
		const auto isMade = util::kFileSystem::CreateNewDirectory(kTest_TestResultFilePath.c_str());
		
		kTest_TestResultFilePath += "Results.txt";
		util::kFileSystem::RemoveFile(kTest_TestResultFilePath.c_str());
#endif // TESTING_ENABLED
	}

	void TesterManager::InitializeMathsTests()
	{
		Add(std::unique_ptr<Tester>(new Maths::VectorsTester()));
	}

	void TesterManager::InitializeUtilityTests()
	{		
		Add(std::unique_ptr<Tester>(new utility::CalendarTester()));
		Add(std::unique_ptr<Tester>(new utility::TimerTester()));
		Add(std::unique_ptr<Tester>(new utility::FileSystemTester()));
		Add(std::unique_ptr<Tester>(new utility::FormatToStringTester()));
		Add(std::unique_ptr<Tester>(new utility::DebugHelpTester()));
		Add(std::unique_ptr<Tester>(new utility::LoggingTester()));
		Add(std::unique_ptr<Tester>(new utility::StringViewTester()));
	}

	void TesterManager::Add(std::unique_ptr<Tester>&& test)
	{
		kTests_TestsUSet.insert(std::move(test));
	}

	void TesterManager::RunAll()
	{
#ifdef TESTING_ENABLED
		for (auto& test : kTests_TestsUSet)
		{			
			const auto resultTest = test->Run() 
				? util::kFormat::FormatToString("Success: Test Name: %s\n\n", test->GetName()) // Success Case
				: util::kFormat::FormatToString("Failure: Test Name: %s\n%s", test->GetName(), test->GetResult().data()); // Fail Case
			
			util::kFileSystem::OutputToFile(kTest_TestResultFilePath.c_str(), resultTest.c_str());
		}
#endif // TESTING_ENABLED
	}

	void TesterManager::ClearAllTests()
	{
		kTests_TestsUSet.clear();
	}
}
