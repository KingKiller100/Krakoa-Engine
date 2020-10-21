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

		const auto filename = "DiffFileName";
		const auto dir = klib::kFileSystem::GetExeDirectory() + "Test Results\\Log Test Dir\\";

		auto testLogger = std::make_unique<Logging>(dir, filename);

		testLogger->SuspendFileLogging();

		testLogger->OutputInitialized("Welcome to logging test");

		testLogger->SetCacheMode(true);

		testLogger->AddBanner(LogEntry("Welcome to the Log Tests!")
			, "Tests", "*", "*", 12);
		auto last = testLogger->GetLastCachedEntry();
		VERIFY(last.find("Welcome to the Log Tests!") != std::string::npos);

		testLogger->AddBanner(LogEntry("BANNER!"), "TEST", "*", "*", 12);
		last = testLogger->GetLastCachedEntry();
		VERIFY(last.find("BANNER!") != std::string::npos);

		testLogger->AddEntry(LogLevel::DBUG, "DEBUG");
		last = testLogger->GetLastCachedEntry();
		VERIFY(last.find("DEBUG!") != std::string::npos);

		testLogger->SetMinimumLoggingLevel(LogLevel::DBUG);

		testLogger->AddEntry(LogLevel::DBUG, "DEBUG");
		last = testLogger->GetLastCachedEntry();
		VERIFY(last.find("DEBUG!") != std::string::npos);

		testLogger->AddEntry(LogLevel::NORM, "NORM");
		last = testLogger->GetLastCachedEntry();
		VERIFY(last.find("NORMAL!") != std::string::npos);

		testLogger->AddEntry(LogLevel::INFO, "INFORMATIVE");
		last = testLogger->GetLastCachedEntry();
		VERIFY(last.find("INFORMATIVE!") != std::string::npos);

		testLogger->AddEntry(LogLevel::WARN, "WARNING");
		last = testLogger->GetLastCachedEntry();
		VERIFY(last.find("WARNING") != std::string::npos);

		testLogger->AddEntry(LogLevel::ERRR, { "ERROR", __FILE__, __LINE__, CalendarInfoSource::LOCAL });
		last = testLogger->GetLastCachedEntry();
		VERIFY(last.find("ERROR!") != std::string::npos);

		testLogger->ErasePrevious(1);
		last = testLogger->GetLastCachedEntry();
		VERIFY(last.find("ERROR!") == std::string::npos);

		testLogger->AddEntry(LogLevel::ERRR, { "ERROR AGAIN", __FILE__, __LINE__, CalendarInfoSource::LOCAL });
		last = testLogger->GetLastCachedEntry();
		VERIFY(last.find("ERROR AGAIN!") != std::string::npos);

		testLogger->ResumeFileLogging();

		testLogger->OutputToFatalFile(LogEntry("FATAL!", __FILE__, __LINE__));

		last = testLogger->GetLastCachedEntry();
		VERIFY(last.find("CHECK LOGGING FILE") != std::string::npos);

		testLogger->FinalOutput();

		fullFilePathToDelete = dir + filename + ".log";
		VERIFY(klib::kFileSystem::CheckFileExists(fullFilePathToDelete.c_str()) == true);

		testLogger->AddEntry(LogLevel::NORM, "End");
		last = testLogger->GetLastCachedEntry();
		VERIFY(last.find("End") == std::string::npos);

		return success;
	}
}
#endif
