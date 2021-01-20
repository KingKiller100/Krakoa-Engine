#include "Precompile.hpp"
#include "EngineLogger.hpp"

#include <Utility/FileSystem/kFileSystem.hpp>
#include <Utility/Calendar/kCalendar.hpp>

namespace krakoa
{
	using namespace klib::kLogs;

	std::unique_ptr<Logging> EngineLogger::pLogger;

	void EngineLogger::CoreInit(const std::string_view& initMsg)
	{
		if (pLogger)
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
		
		pLogger = std::make_unique<Logging>(dir, filename, extension, name);
		pLogger->SetCacheMode(false);
		
		pLogger->GetConsole().SetDebugStringOutput(true);
		
		pLogger->AddRaw(padding);
		pLogger->AddRaw(stamp);
		pLogger->AddRaw(initMsg);
		pLogger->AddRaw(padding);
		
		pLogger->AddRaw();
	}

	void EngineLogger::ShutDown()
	{
		GetLogger().AddRaw(R"(
************************************************************************
                              Logging Concluded
************************************************************************)"
);
		pLogger->FinalOutput(false);
	}

	Logging& EngineLogger::GetLogger()
	{
		return *pLogger;
	}
}
