#include "Precompile.hpp"
#include "CoreLogger.hpp"

#include <Utility/File System/kFileSystem.hpp>

namespace krakoa
{
	using namespace klib::kLogs;

	std::unique_ptr<Logging> CoreLogger::pCoreLogger;

	void CoreLogger::CoreInit()
	{
		if (pCoreLogger)
			return;

		const auto dir = klib::kFileSystem::GetExeDirectory() + "Logs\\";
		constexpr auto name = "Krakoa";
		constexpr auto filename = "Krakoa Engine";
		
		pCoreLogger = std::make_unique<Logging>(dir, filename, name);
		pCoreLogger->ToggleConsoleEnabled();
		pCoreLogger->OutputInitialized("Welcome to the engine");
	}

	Logging& CoreLogger::GetCoreLogger()
	{
		return *pCoreLogger;
	}
}
