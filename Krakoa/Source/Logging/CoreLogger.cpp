#include "Precompile.hpp"
#include "CoreLogger.hpp"

#include <Utility/FileSystem/kFileSystem.hpp>
#include <Utility/Calendar/kCalendar.hpp>

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
		constexpr auto extension = ".log";

		const auto padding = std::string(72, '*');
		const auto spacing = std::string(12, ' ');

		const GregorianCalendar now(CalendarInfoSourceType::LOCAL);
		const auto timeStr = now.GetTime().ToString(Time::MILLIS);
		const auto dateStr = now.GetDate().ToString("mmm ddd yyyy");
		const auto stamp = spacing + dateStr + spacing + timeStr + spacing;
		
		pCoreLogger = std::make_unique<Logging>(dir, filename, extension, name);
		pCoreLogger->SetCacheMode(false);
		
		pCoreLogger->GetConsole().SetDebugStringOutput(true);
		
		pCoreLogger->AddRaw(padding);
		pCoreLogger->AddRaw(stamp);
		pCoreLogger->AddRaw(initMsg);
		pCoreLogger->AddRaw(padding);
		
		pCoreLogger->AddRaw();
	}

	void CoreLogger::ShutDown()
	{
		GetLogger().AddRaw(R"(
************************************************************************
                              Logging Concluded
************************************************************************)"
);
		pCoreLogger->FinalOutput(false);
	}

	Logging& CoreLogger::GetLogger()
	{
		return *pCoreLogger;
	}
}
