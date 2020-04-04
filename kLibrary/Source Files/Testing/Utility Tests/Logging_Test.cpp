#include "pch.hpp"
#include "Logging_Test.hpp"

#include "../../Utility/Logging/kLogging_Class.hpp"
#include "../../Utility/File System/kFileSystem.hpp"
#include "../../Utility/Calendar/kCalendar.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{		
	LoggingTester::LoggingTester()
		: Tester("Logging Test")
	{	}

	LoggingTester::~LoggingTester()
		= default;

	void LoggingTester::Test()
	{
		VERIFY(LogTest());

		klib::kFileSystem::RemoveFile(fullFilePathToDelete.data());
		fullFilePathToDelete.erase(fullFilePathToDelete.find_last_of('\\'));
		klib::kFileSystem::DeleteDirectory(fullFilePathToDelete.data());
	}

	bool LoggingTester::LogTest() noexcept
	{
		using namespace klib::kLogs;

		auto testLogger = std::make_unique<Logging>();

		testLogger->OutputInitialized();

		const auto dir = klib::kFileSystem::GetCurrentWorkingDirectory() + "Test Results\\Log Test Dir\\";
		testLogger->ChangeOutputDirectory(dir);

		testLogger->SuspendFileLogging();

		const auto previousLogFile = dir + "Logs - " + klib::kCalendar::GetDateInNumericalFormat(false) + ".log";
		klib::kFileSystem::RemoveFile(previousLogFile.c_str());

		const auto filename = "DiffFileName";
		testLogger->ChangeFilename(filename);

		testLogger->SetCacheMode(true);

		testLogger->AddEntryBanner("Welcome to the Log Tests!", "Tests");
		auto last = testLogger->GetLastCachedEntry();
		VERIFY(last.find("Welcome to the Log Tests!") != std::string::npos);

		testLogger->AddEntryBanner("BANNER!", "TEST");
		last = testLogger->GetLastCachedEntry();
		VERIFY(last.find("BANNER!") != std::string::npos);

		testLogger->AddEntry("DEBUG!", LLevel::DBUG);
		last = testLogger->GetLastCachedEntry();
		VERIFY(last.find("DEBUG!") == std::string::npos);

		testLogger->SetMinimumLoggingLevel(LLevel::DBUG);

		testLogger->AddEntry("DEBUG!", LLevel::DBUG);
		last = testLogger->GetLastCachedEntry();
		VERIFY(last.find("DEBUG!") != std::string::npos);

		testLogger->AddEntry("NORMAL!");
		last = testLogger->GetLastCachedEntry();
		VERIFY(last.find("NORMAL!") != std::string::npos);

		testLogger->AddEntry("INFORMATIVE!", LLevel::INFO);
		last = testLogger->GetLastCachedEntry();
		VERIFY(last.find("INFORMATIVE!") != std::string::npos);

		testLogger->AddEntry("WARN", LLevel::WARN);
		last = testLogger->GetLastCachedEntry();
		VERIFY(last.find("WARN") != std::string::npos);

		testLogger->AddEntry("ERROR!", LLevel::ERRR, __FILE__, __LINE__);
		last = testLogger->GetLastCachedEntry();
		VERIFY(last.find("ERROR!") != std::string::npos);

		testLogger->ErasePreviousCacheEntries(1);
		last = testLogger->GetLastCachedEntry();
		VERIFY(last.find("ERROR!") == std::string::npos);

		testLogger->AddEntry("ERROR AGAIN!", LLevel::ERRR, __FILE__, __LINE__);
		last = testLogger->GetLastCachedEntry();
		VERIFY(last.find("ERROR AGAIN!") != std::string::npos);

		testLogger->ResumeFileLogging();

		testLogger->OutputToFatalFile("FATAL!", __FILE__, __LINE__);

		last = testLogger->GetLastCachedEntry();
		VERIFY(last.find("CHECK LOGGING FILE") != std::string::npos);

		testLogger->FinalOutput();

		fullFilePathToDelete = dir + filename + ".log";
		VERIFY(klib::kFileSystem::CheckFileExists(fullFilePathToDelete.c_str()) == true);

		testLogger->AddEntry("end");
		last = testLogger->GetLastCachedEntry();
		VERIFY(last.find("end") == std::string::npos);

		return success;
	}
}
#endif
