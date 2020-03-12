#include <Precompile.hpp>
#include <Core/Logging/CoreLogger.hpp>

namespace krakoa
{
	using namespace klib::kLogs;

	std::unique_ptr<Logging> CoreLogger::coreLogger;

	void CoreLogger::CoreInit()
	{
		coreLogger = std::make_unique<Logging>();
		coreLogger->SetName("Krakoa");
		coreLogger->ChangeFilename("Krakoa Engine");
		coreLogger->ToggleSubSystemEnabled();
		coreLogger->OutputInitialized();
	}
}
