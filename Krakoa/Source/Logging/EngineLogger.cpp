﻿#include "Precompile.hpp"
#include "EngineLogger.hpp"

#include "../Config/GlobalConfig.hpp"

#include <Utility/FileSystem/kFileSystem.hpp>
#include <Utility/Calendar/kCalendar.hpp>

namespace krakoa
{
	Solo_Ptr<klib::Logging> EngineLogger::pLogger;

	template<typename T>
	T GetFromLogConfig(const std::string& key)
	{
		const auto& globalConfig = configuration::GlobalConfig::Reference();
		const auto value = globalConfig.Get<T>("Logging", key);
		return value;
	}
	
	void EngineLogger::SetMinimumLogLevelFromConfig()
	{
		const auto logLevelStr = GetFromLogConfig<std::string_view>("Level");
		const auto min_level = klib::LogLevel::FromString(klib::ToUpper(logLevelStr));
		pLogger->SetMinimumLoggingLevel(min_level);
	}

	void EngineLogger::RemoveOldFile(const std::filesystem::path& fileToDelete)
	{
		const auto entry = std::filesystem::directory_entry(fileToDelete);

		if (!entry.exists())
			return;

		const auto maxBytes = GetFromLogConfig<size_t>("MaxBytes");
		const auto fileSize = entry.file_size();
		const auto tooBig = fileSize > maxBytes;
		
		const auto maxDays = GetFromLogConfig<size_t>("MaxDays");
		const auto lastWrite = entry.last_write_time();
		const auto now = std::filesystem::_File_time_clock::now();
		const auto diff = now - lastWrite;
		const auto totalHours = std::chrono::duration_cast<std::chrono::hours>(diff);
		const auto totalDays = totalHours / 24.0;
		const auto tooOld = maxDays < totalDays.count();

		if (tooBig || tooOld)
			klib::Remove(fileToDelete);
	}

	void EngineLogger::CoreInit(const std::string_view& initMsg)
	{
		if (pLogger)
			return;

		std::filesystem::path dir(klib::kFileSystem::GetExeDirectory());
		dir /= "Logs\\";

		constexpr auto name = "Krakoa";
		constexpr auto filename = "KrakoaEngine";
		constexpr auto extension = ".log";

		const auto path = dir / (std::string(filename) + extension);

		RemoveOldFile(path);
		
		const auto padding = std::string(72, '*');
		const auto spacing = std::string(12, ' ');

		const klib::GregorianCalendar now(klib::CalendarInfoSourceType::LOCAL);
		const auto timeStr = now.GetTime().ToString(klib::Time::MILLIS);
		const auto dateStr = now.GetDate().ToString("mmm ddd yyyy");
		const auto stamp = spacing + dateStr + spacing + timeStr + spacing;
		
		pLogger = Make_Solo<klib::Logging>(path, name);

		SetMinimumLogLevelFromConfig();
		
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