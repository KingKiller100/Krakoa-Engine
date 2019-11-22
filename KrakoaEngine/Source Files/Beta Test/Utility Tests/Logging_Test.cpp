#include "Precompile.h"
#include "Logging_Test.h"

#include "../../Utility/File System/kFileSystem.h"
#include "../../Utility/Logging/kLogging.h"

namespace kTest::utility
{
	using namespace util;
	using namespace util::kFileSystem;

	
	LoggingTester::LoggingTester()
		: Tester("Logging Test")
	{	}

	LoggingTester::~LoggingTester()
		= default;

	void LoggingTester::Test()
{
		INIT_LOGS();

		const auto dir = kFileSystem::GetCurrentWorkingDirectory<char>() + "Change Dir\\";		
		CHANGE_LOGS_DESTINATION(dir);

		const auto filename = "DiffFileName";
		CHANGE_LOGS_FILENAME(filename);
		
		LOG_BANNER("Welcome to the Krakoa Engine!", "Krakoa");
		auto last = GET_LAST_LOG_ENTRY();
		VERIFY(last.find("Welcome to the Krakoa Engine!") != std::string::npos);
		
		LOG_BANNER("BANNER!", "TEST");
		last = GET_LAST_LOG_ENTRY();
		VERIFY(last.find("BANNER!") != std::string::npos)

		LOG_ENTRY("NORMAL!", kLogs::LogLevel::NORM);
		last = GET_LAST_LOG_ENTRY();
		VERIFY(last.find("NORMAL!") != std::string::npos)

		LOG_ENTRY("INFORMATIVE!", kLogs::LogLevel::INFO);
		last = GET_LAST_LOG_ENTRY();
		VERIFY(last.find("INFORMATIVE!") != std::string::npos)

		LOG_ENTRY("Done", kLogs::LogLevel::WARN);
		last = GET_LAST_LOG_ENTRY();
		VERIFY(last.find("Done") != std::string::npos)

		LOG_ENTRY("ERROR!", kLogs::LogLevel::ERRR);
		last = GET_LAST_LOG_ENTRY();
		VERIFY(last.find("ERROR!") != std::string::npos);

		ERASE_LOG_ENTRIES(1);
		last = GET_LAST_LOG_ENTRY();
		VERIFY(last.find("ERROR!") == std::string::npos);
				
		LOG_ENTRY("ERROR AGAIN!", kLogs::LogLevel::ERRR);
		last = GET_LAST_LOG_ENTRY();
		VERIFY(last.find("ERROR AGAIN!") == std::string::npos);

		
		FATAL("FATAL!");
		last = GET_LAST_LOG_ENTRY();
		VERIFY(last.find("EMPTY") == std::string::npos);
		
		FLUSH_LOGS();
		END_LOGGING();

		const auto fullFilePathToDelete = dir + filename + ".log";
		VERIFY(CheckFileExists(fullFilePathToDelete.c_str()) == true);
		
		LOG_ENTRY("end", kLogs::LogLevel::NORM);	
		last = GET_LAST_LOG_ENTRY();
		VERIFY(last.find("end") == std::string::npos);
	}
}
