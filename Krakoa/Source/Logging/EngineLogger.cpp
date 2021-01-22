#include "Precompile.hpp"
#include "EngineLogger.hpp"

#include "../Config/GlobalConfig.hpp"

#include <Utility/FileSystem/kFileSystem.hpp>
#include <Utility/Calendar/kCalendar.hpp>

namespace krakoa
{
	std::unique_ptr<klib::Logging> EngineLogger::pLogger;

	void EngineLogger::SetLogLevelFromConfig()
	{
		const auto& globalConfig = configuration::GlobalConfig::Reference();
		const auto logLevelRaw = globalConfig.Get<std::string>("Logging", "Level");
		const auto min_level = klib::LogLevel::FromString(klib::ToUpper(logLevelRaw));
		pLogger->SetMinimumLoggingLevel(min_level);
	}
	
	void EngineLogger::CoreInit(const std::string_view& initMsg)
	{
		if (pLogger)
			return;

		std::string dir(klib::kFileSystem::GetExeDirectory());
		dir += "Logs\\";

		constexpr auto name = "Krakoa";
		constexpr auto filename = "KrakoaEngine";
		constexpr auto extension = ".log";

		const auto padding = std::string(72, '*');
		const auto spacing = std::string(12, ' ');

		const klib::GregorianCalendar now(klib::CalendarInfoSourceType::LOCAL);
		const auto timeStr = now.GetTime().ToString(klib::Time::MILLIS);
		const auto dateStr = now.GetDate().ToString("mmm ddd yyyy");
		const auto stamp = spacing + dateStr + spacing + timeStr + spacing;
		
		pLogger = std::make_unique<klib::Logging>(dir, filename, extension, name);

		SetLogLevelFromConfig();
		
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

	klib::Logging& EngineLogger::GetLogger()
	{
		return *pLogger;
	}
}
