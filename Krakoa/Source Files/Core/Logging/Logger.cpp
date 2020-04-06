#include "Precompile.hpp"
#include "Logger.hpp"

namespace krakoa
{
	using namespace klib::kLogs;

	std::unique_ptr<Logging> Logger::pClientLogger;

	void Logger::ClientInit(const char* name)
	{
		pClientLogger = std::make_unique<Logging>();
		pClientLogger->SetName(name);
		pClientLogger->ChangeFilename(name);
		pClientLogger->OutputInitialized();
	}

	klib::kLogs::Logging & Logger::GetClientLogger()
	{
		return *pClientLogger;
	}
}
