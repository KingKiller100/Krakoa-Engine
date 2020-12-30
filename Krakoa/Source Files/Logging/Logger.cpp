#include "Precompile.hpp"
#include "Logger.hpp"

#include <Utility/FileSystem/kFileSystem.hpp>

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
		
		constexpr auto extension = ".log";

		pClientLogger = std::make_unique<Logging>(dir, name, extension, name);
		pClientLogger->ToggleConsoleEnabled();
		
		const auto padding = std::string(72, '*');
		pClientLogger->AddRaw(padding);
		pClientLogger->AddRaw("Welcome client!");
		pClientLogger->AddRaw(padding);
	}

	Logging & Logger::GetLogger()
	{
		return *pClientLogger;
	}
}
