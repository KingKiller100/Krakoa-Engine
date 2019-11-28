#include <Precompile.h>
#include <Testing/TesterManager.hpp>

#include <Testing/Tester.hpp>

// Maths Tests
#include <Testing/Maths Tests/Vectors_Test.hpp>

// Utility Tests
#include <Testing/Utility Tests/Clock_Test.hpp>
#include <Testing/Utility Tests/Logging_Test.hpp>
#include <Testing/Utility Tests/Calendar_Test.hpp>
#include <Testing/Utility Tests/DebugHelp_Test.hpp>
#include <Testing/Utility Tests/FileSystem_Test.hpp>
#include <Testing/Utility Tests/StringView_Test.hpp>
#include <Testing/Utility Tests/FormatToString_Test.hpp>

// File System to output test results
#include <Utility/File System/kFileSystem.hpp>

#include <unordered_map>

namespace kTest
{
	std::unordered_map<const char*, std::unique_ptr<Tester>> kTests_TestsUMap;
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
		kTest_TestResultFilePath = util::kFileSystem::GetCurrentWorkingDirectory<char>() + "TestResults\\";
		util::kFileSystem::CreateNewDirectory(kTest_TestResultFilePath.c_str());
		
		kTest_TestResultFilePath += "Results.txt";
		util::kFileSystem::RemoveFile(kTest_TestResultFilePath.c_str());
	}

	void TesterManager::InitializeMathsTests()
	{
		Add(new Maths::VectorsTester());
	}

	void TesterManager::InitializeUtilityTests()
	{		
		Add(new utility::CalendarTester());
		Add(new utility::ClockTester());
		Add(new utility::FileSystemTester());
		Add(new utility::FormatToStringTester());
		Add(new utility::DebugHelpTester());
		Add(new utility::LoggingTester());
		Add(new utility::StringViewTester());
	}

	void TesterManager::Add(Tester* test)
	{
		kTests_TestsUMap.insert(std::make_pair(test->GetName(), test));
	}

	void TesterManager::RunAll()
	{
		for (auto& test : kTests_TestsUMap)
		{			
			std::string resultTest;
			
			if (test.second->Run())
			{
				resultTest = util::kFormat::FormatToString("Success: Test Name: %s\n\n", test.first);
			}
			else
			{
				resultTest = util::kFormat::FormatToString("Failure: Test Name: %s\n%s", test.first, test.second->GetResult().data());
			}

			util::kFileSystem::OutputToFile(kTest_TestResultFilePath.c_str(), resultTest.c_str());
		}
	}

	void TesterManager::ClearAllTests()
	{
		kTests_TestsUMap.clear();
	}
}
