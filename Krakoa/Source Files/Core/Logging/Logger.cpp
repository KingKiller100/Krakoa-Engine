#include <Precompile.hpp>
#include <Core/Logging/Logger.hpp>

namespace krakoa
{
	using namespace klib::kLogs;

	std::unique_ptr<Logging> Logger::clientLogger;

	void Logger::ClientInit(const char* name)
	{
		clientLogger = std::make_unique<Logging>();
		clientLogger->SetName(name);
		clientLogger->ChangeFilename(name);
		clientLogger->OutputInitialized();
	}
}
