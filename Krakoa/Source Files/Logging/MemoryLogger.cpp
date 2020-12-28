#include "Precompile.hpp"
#include "MemoryLogger.hpp"

#include <Utility/FileSystem/kFileSystem.hpp>

namespace memory
{
	using namespace klib::kLogs;

	std::unique_ptr<Logging> MemoryLogger::pMemoryLogger;

	void MemoryLogger::Init(const std::string_view& openingMsg)
	{
		if (pMemoryLogger)
			return;

		std::string dir(klib::kFileSystem::GetExeDirectory());
		dir += "Logs\\";
		constexpr auto filename = "Memory";
		constexpr auto name = filename;
		constexpr auto extension = ".log";
		
		pMemoryLogger = std::make_unique<Logging>(dir, filename, extension, name);
		pMemoryLogger->ToggleConsoleEnabled();
		pMemoryLogger->OutputInitialized(openingMsg);
	}

	Logging & MemoryLogger::GetLogger()
	{
		return *pMemoryLogger;
	}
}
