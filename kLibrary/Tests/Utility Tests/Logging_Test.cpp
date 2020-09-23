#include "pch.hpp"
#include "Logging_Test.hpp"


#include "../../Source Files/Utility/File System/kFileSystem.hpp"
#include "../../Source Files/Utility/Logging/kLogging_Class.hpp"

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
		VERIFY(LogTest() == true);

		klib::kFileSystem::RemoveFile(fullFilePathToDelete.data());
		fullFilePathToDelete.erase(fullFilePathToDelete.find_last_of('\\'));
		klib::kFileSystem::DeleteDirectory(fullFilePathToDelete.data());
	}

	bool LoggingTester::LogTest()
	{
		using namespace klib::kLogs;

		auto testLogger = std::make_unique<Logging>();

		const auto filename = "DiffFileName";
		const auto dir = klib::kFileSystem::GetExeDirectory() + "Test Results\\Log Test Dir\\";

		testLogger->ChangeOutputPath(dir, filename);

		testLogger->SuspendFileLogging();

		testLogger->OutputInitialized();

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