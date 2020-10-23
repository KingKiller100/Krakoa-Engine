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

		std::string dir(klib::kFileSystem::GetExeDirectory());
		dir += "Logs\\";
		
		pClientLogger = std::make_unique<Logging>(dir, name, name);
		pClientLogger->OutputInitialized("Welcome client!");
	}

	Logging & Logger::GetLogger()
	{
		return *pClientLogger;
	}
}
