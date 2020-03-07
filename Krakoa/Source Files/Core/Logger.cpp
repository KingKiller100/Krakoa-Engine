#include <Precompile.hpp>
#include <Core/Logger.hpp>

namespace krakoa
{
	using namespace klib::kLogs;

	std::unique_ptr<Logging> Logger::coreLogger;
	std::unique_ptr<Logging> Logger::clientLogger;

	void krakoa::Logger::CoreInit()
	{
		coreLogger = std::make_unique<Logging>();
		coreLogger->InitializeLogging(LLevel::NORM);
	}

	void Logger::ClientInit()
	{
		clientLogger = std::make_unique<Logging>();
		clientLogger->InitializeLogging(LLevel::NORM);
	}
}
