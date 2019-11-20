#include "Precompile.h"
#include "Logging_Test.h"

#include "../../Utility/File System/kFileSystem.h"
#include "../../Utility/Logging/kLogging.h"

namespace kTest::utility
{
	using namespace util;

	LoggingTester::LoggingTester()
		: Tester("Logging Test")
	{	}

	LoggingTester::~LoggingTester()
		= default;

	bool LoggingTester::Test()
	{
		INIT_LOGS;
		CHANGE_LOGS_DESTINATION(kFileSystem::GetCurrentWorkingDirectory<char>() + "Change Dir\\");
		CHANGE_LOGS_FILENAME("DiffFileName");
		LOG_BANNER("Welcome to the Krakoa Engine!", "Krakoa");
		LOG_BANNER("BANNER!", "TEST");
		LOG_ENTRY("NORMAL!", kLogs::LogLevel::NORM);
		LOG_ENTRY("INFORMATIVE!", kLogs::LogLevel::INFO);
		LOG_ENTRY("Done", kLogs::LogLevel::WARN);
		LOG_ENTRY("ERROR!", kLogs::LogLevel::ERRR);
		auto last = GET_LAST_LOG_ENTRY;
		VERIFY(last.find("ERROR!") != std::string::npos);
		ERASE_LOG_ENTRIES(1);
		LOG_ENTRY("ERROR AGAIN!", kLogs::LogLevel::ERRR);
		FATAL("FATAL!");
		FLUSH_LOGS;
		END_LOGGING;
		LOG_ENTRY("end", kLogs::LogLevel::NORM);		

		return true;
	}
}
