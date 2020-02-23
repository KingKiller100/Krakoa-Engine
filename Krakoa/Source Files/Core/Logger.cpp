#include <Precompile.hpp>
#include <Core/Logger.hpp>

namespace krakoa
{
	std::unique_ptr<Logging> Logger::coreLogger;
	std::unique_ptr<Logging> Logger::clientLogger;

	void krakoa::Logger::Init()
	{
		coreLogger = std::make_unique<Logging>();
		clientLogger = std::make_unique<Logging>();

		coreLogger->InitializeLogging(LLevel::NORM);
		clientLogger->InitializeLogging(LLevel::NORM);
	}
}
