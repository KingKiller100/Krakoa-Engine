#include <pch.hpp>
#include <Testing/TesterManager.hpp>

#include <Testing/Tester.hpp>

// Maths Tests
#include <Testing/Maths Tests/Matrix_Test.hpp>
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

#ifdef TESTING_ENABLED
namespace kTest
{
	TesterManager* TesterManager::pInstance = nullptr;

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
		kTest_TestResultFilePath = klib::kFileSystem::GetCurrentWorkingDirectory<char>() + "Test Results\\";
		const auto isMade = klib::kFileSystem::CreateNewDirectory(kTest_TestResultFilePath.c_str());
		
		kTest_TestResultFilePath += "Results.txt";
		klib::kFileSystem::RemoveFile(kTest_TestResultFilePath.c_str());
	}

	void TesterManager::InitializeMathsTests()
	{
		Add(new Maths::MatricesTester());
		Add(new Maths::VectorsTester());
	}

	void TesterManager::InitializeUtilityTests()
	{		
		Add(new utility::CalendarTester());
		Add(new utility::TimerTester());
		Add(new utility::FileSystemTester());
		Add(new utility::FormatToStringTester());
		Add(new utility::DebugHelpTester());
		Add(new utility::LoggingTester());
		Add(new utility::StringViewTester());
	}

	void TesterManager::Add(Tester* test)
	{
		kTests_TestsUSet.insert(std::unique_ptr<Tester>(std::move(test)));
	}

	void TesterManager::RunAll()
	{
		for (auto& test : kTests_TestsUSet)
		{
			const auto resultTest = test->Run() 
				? klib::kFormat::ToString("Success: Test Name: %s\n\n", test->GetName()) // Success Case
				: klib::kFormat::ToString("Failure: Test Name: %s\n%s", test->GetName(), test->GetFailureData().data()); // Fail Case

			klib::kFileSystem::OutputToFile(kTest_TestResultFilePath.c_str(), resultTest.c_str());
		}
	}

	void TesterManager::ClearAllTests()
	{
		kTests_TestsUSet.clear();
	}
	
	TesterManager& TesterManager::Get()
	{
		if (!pInstance)
			pInstance = new TesterManager();
		return *pInstance;
	}
}
#endif // TESTING_ENABLED
