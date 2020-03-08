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
		coreLogger->OutputInitialized();
	}

	void Logger::ClientInit(const char* name)
	{
		clientLogger = std::make_unique<Logging>();
		clientLogger->SetName(name);
		clientLogger->ChangeFilename(name);
	}
}
