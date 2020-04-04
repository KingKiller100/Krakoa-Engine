#include "Precompile.hpp"
#include "CoreLogger.hpp"

namespace krakoa
{
	using namespace klib::kLogs;

	std::unique_ptr<Logging> CoreLogger::pCoreLogger;

	void CoreLogger::CoreInit()
	{
		pCoreLogger = std::make_unique<Logging>();
		pCoreLogger->SetName("Krakoa");
		pCoreLogger->ChangeFilename("Krakoa Engine");
		pCoreLogger->ToggleSubSystemEnabled();
		pCoreLogger->OutputInitialized();
	}
}
