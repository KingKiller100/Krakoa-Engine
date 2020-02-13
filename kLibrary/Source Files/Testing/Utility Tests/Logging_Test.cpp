#include <pch.hpp>
#include <Testing/Utility Tests/Logging_Test.hpp>

#include <Utility/File System/kFileSystem.hpp>
#include <Utility/Logging/kLogging.hpp>

namespace kTest::utility
{		
	LoggingTester::LoggingTester()
		: Tester("Logging Test")
	{	}

	LoggingTester::~LoggingTester()
		= default;

	void LoggingTester::Test()
	{
		using namespace klib;

		INIT_LOGS();

		const auto dir = klib::kFileSystem::GetCurrentWorkingDirectory() + "Test Results\\Log Test Dir\\";
		CHANGE_LOGS_DESTINATION(dir);

		const auto filename = "DiffFileName";
		CHANGE_LOGS_FILENAME(filename);
		
		LOG_BANNER("Welcome to the Krakoa Engine!", "Krakoa");
		auto last = GET_LAST_LOG_ENTRY();
		VERIFY(last.find("Welcome to the Krakoa Engine!") != std::string::npos);
		
		LOG_BANNER("BANNER!", "TEST");
		last = GET_LAST_LOG_ENTRY();
		VERIFY(last.find("BANNER!") != std::string::npos)

		LOG_NORM("NORMAL!");
		last = GET_LAST_LOG_ENTRY();
		VERIFY(last.find("NORMAL!") != std::string::npos)

		LOG_INFO("INFORMATIVE!");
		last = GET_LAST_LOG_ENTRY();
		VERIFY(last.find("INFORMATIVE!") != std::string::npos)

		LOG_NORM("Done");
		last = GET_LAST_LOG_ENTRY();
		VERIFY(last.find("Done") != std::string::npos)

		LOG_ERRR("ERROR!");
		last = GET_LAST_LOG_ENTRY();
		VERIFY(last.find("ERROR!") != std::string::npos);

		ERASE_LOG_ENTRIES(1);
		last = GET_LAST_LOG_ENTRY();
		VERIFY(last.find("ERROR!") == std::string::npos);
				
		LOG_ERRR("ERROR AGAIN!");
		last = GET_LAST_LOG_ENTRY();
		VERIFY(last.find("ERROR AGAIN!") != std::string::npos);
				
		FATAL("FATAL!");

		last = GET_LAST_LOG_ENTRY();
		VERIFY(last.find("EMPTY") != std::string::npos);
		
		FLUSH_LOGS();
		END_LOGGING();

		const auto fullFilePathToDelete = dir + filename + ".log";
		VERIFY(klib::kFileSystem::CheckFileExists(fullFilePathToDelete.c_str()) == true);
		
		LOG_NORM("end");
		last = GET_LAST_LOG_ENTRY();
		VERIFY(last.find("end") == std::string::npos);

		kFileSystem::RemoveFile(fullFilePathToDelete.data());
		kFileSystem::DeleteDirectory(dir.data());
	}
}
