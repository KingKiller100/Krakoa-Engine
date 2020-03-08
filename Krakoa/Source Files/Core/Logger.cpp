#include <Precompile.hpp>
#include <Core/Logger.hpp>

namespace krakoa
{
	using namespace klib::kLogs;

	std::unique_ptr<Logging> Logger::coreLogger;
	std::unique_ptr<Logging> Logger::clientLogger;

	void Logger::CoreInit()
	{
		coreLogger = std::make_unique<Logging>();
		coreLogger->SetName("Krakoa");
		coreLogger->ChangeFilename("Krakoa Engine");
		//coreLogger->SetMinimumLoggingLevel(LLevel::NORM);
		coreLogger->InitializeLogging();
	}

	void Logger::ClientInit()
	{
		clientLogger = std::make_unique<Logging>();
		clientLogger->SetName("App");
		clientLogger->ChangeFilename("App");
	}
}
