#include "Precompile.hpp"
#include "Logger.hpp"

#include <Utility/File System/kFileSystem.hpp>

namespace krakoa
{
	using namespace klib::kLogs;

	std::unique_ptr<Logging> Logger::pClientLogger;

	void Logger::ClientInit(const char* name)
	{
		if (pClientLogger)
			return;

		const auto dir = klib::kFileSystem::GetExeDirectory() + "Logs\\";
		
		pClientLogger = std::make_unique<Logging>(dir.c_str(), name, name);
		pClientLogger->OutputInitialized("Welcome client!");
	}

	Logging & Logger::GetLogger()
	{
		return *pClientLogger;
	}
}
