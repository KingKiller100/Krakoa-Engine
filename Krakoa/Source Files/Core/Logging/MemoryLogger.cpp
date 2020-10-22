#include "Precompile.hpp"
#include "MemoryLogger.hpp"

#include <Utility/File System/kFileSystem.hpp>

namespace memory
{
	using namespace klib::kLogs;

	std::unique_ptr<Logging> MemoryLogger::pMemoryLogger;

	void MemoryLogger::Init(const std::string_view& openingMsg)
	{
		if (pMemoryLogger)
			return;

		const auto dir = klib::kFileSystem::GetExeDirectory() + "\\Logs";
		constexpr auto filename = "Memory";
		constexpr auto name = filename;
		
		pMemoryLogger = std::make_unique<Logging>(dir, filename, name);
		pMemoryLogger->OutputInitialized(openingMsg);
	}

	Logging & MemoryLogger::GetLogger()
	{
		return *pMemoryLogger;
	}
}
