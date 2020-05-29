#include "Precompile.hpp"
#include "MemoryLogger.hpp"

namespace memory
{
	using namespace klib::kLogs;

	std::unique_ptr<Logging> MemoryLogger::pMemoryLogger;

	void MemoryLogger::Init()
	{
		if (pMemoryLogger)
			return;

		pMemoryLogger = std::make_unique<Logging>();
		pMemoryLogger->SetName("Memory");
		pMemoryLogger->ChangeFilename("Memory");
		pMemoryLogger->OutputInitialized();
	}

	Logging & MemoryLogger::GetLogger()
	{
		return *pMemoryLogger;
	}
}
