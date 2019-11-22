#include "Precompile.h"
#include "TesterManager.h"

#include "Tester.h"

// Maths Tests
#include "Maths Tests/Vectors_Test.h"

// Utility Tests
#include "Utility Tests/Calendar_Test.h"
#include "Utility Tests/Clock_Test.h"
#include "Utility Tests/DebugHelp_Test.h"
#include "Utility Tests/FileSystem_Test.h"
#include "Utility Tests/Logging_Test.h"
#include "Utility Tests/StringView_Test.h"

// File System to output test results
#include "../Utility/File System/kFileSystem.h"

#include <unordered_map>

namespace kTest
{
	std::unordered_map<const char*, std::unique_ptr<Tester>> kTests_Tests;
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
		Add(new utility::DebugHelpTester());
		Add(new utility::LoggingTester());
		Add(new utility::StringViewTester());
	}

	void TesterManager::Add(Tester* test)
	{
		kTests_Tests.insert(std::make_pair(test->GetName(), test));
	}

	void TesterManager::RunAll()
	{
		for (auto& test : kTests_Tests)
		{			
			std::string resultTest;
			
			if (test.second->Run())
			{
				resultTest = util::kFormatToString::FormatToString("Success: Test Name: %s\n", test.first);
			}
			else
			{
				resultTest = test.second->GetResult();
			}

			util::kFileSystem::OutputToFile(kTest_TestResultFilePath.c_str(), resultTest.c_str());
		}
	}

	void TesterManager::ClearAllTests()
	{
		kTests_Tests.clear();
	}
}
