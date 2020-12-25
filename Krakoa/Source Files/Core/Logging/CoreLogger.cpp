#include "Precompile.hpp"
#include "CoreLogger.hpp"

#include <Utility/FileSystem/kFileSystem.hpp>

namespace krakoa
{
	using namespace klib::kLogs;

	std::unique_ptr<Logging> CoreLogger::pCoreLogger;

	void CoreLogger::CoreInit(const std::string_view& initMsg)
	{
		if (pCoreLogger)
			return;

		std::string dir(klib::kFileSystem::GetExeDirectory());
		dir += "Logs\\";
		
		constexpr auto name = "Krakoa";
		constexpr auto filename = "Krakoa Engine";
		
		pCoreLogger = std::make_unique<Logging>(dir, filename, name);
		pCoreLogger->OutputInitialized(initMsg);
	}

	Logging& CoreLogger::GetCoreLogger()
	{
		return *pCoreLogger;
	}
}
