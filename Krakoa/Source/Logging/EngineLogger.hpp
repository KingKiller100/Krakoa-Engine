#pragma once

#include "../Core/EngineMacros.hpp"
#include "../Core/PointerTypes.hpp"

#include <Utility/Logging/kLogging.hpp>

#include <memory>


namespace krakoa
{
	namespace configuration
	{
		class ConfigValueMap;
	}

	class KRAKOA_API EngineLogger
	{
	public:
		// Engine side
		static void CoreInit(const std::string_view& initMsg);
		static klib::kLogs::Logging& GetLogger();
		static void ShutDown();

	private:
		static void SetMinimumLogLevelFromConfig(const configuration::ConfigValueMap& valueMap);
		static void RemoveOldFile(const configuration::ConfigValueMap& valueMap, const std::filesystem::path& fileToDelete);
		
	private:
		static Solo_Ptr<klib::kLogs::Logging> pLogger;
	};
}

// Initializer
#define KRK_LOG_LVL_DBG                                            ::klib::kLogs::LogLevel::DBG
#define KRK_LOG_LVL_NRM                                            ::klib::kLogs::LogLevel::NRM
#define KRK_LOG_LVL_INF                                            ::klib::kLogs::LogLevel::INF
#define KRK_LOG_LVL_WRN                                            ::klib::kLogs::LogLevel::WRN
#define KRK_LOG_LVL_ERR                                            ::klib::kLogs::LogLevel::ERR


// ENGINE SIDE Log Macros
#define KRK_INIT_LOGS(msg)                                          ::krakoa::EngineLogger::CoreInit(msg)
#define KRK_SET_LOG_MIN(minLvl)                                     ::krakoa::EngineLogger::GetLogger().SetMinimumLoggingLevel(minLvl)
#define KRK_GET_LOG_PATH()                                          ::krakoa::EngineLogger::GetLogger().GetFile().GetPath()
#define KRK_TOGGLE_LOGGING()                                        ::krakoa::EngineLogger::GetLogger().ToggleLoggingEnabled()
#define KRK_DISABLE_FILE_OUTPUT()                                   ::krakoa::EngineLogger::GetLogger().SetCacheMode(true)
#define KRK_ENABLE_FILE_OUTPUT()                                    ::krakoa::EngineLogger::GetLogger().SetCacheMode(false)
#define KRK_CHANGE_LOGS_DESTINATION(destination)                    ::krakoa::EngineLogger::GetLogger().GetFile().SetPath(destination)
#define KRK_CHANGE_LOGS_FILENAME(name)                              ::krakoa::EngineLogger::GetLogger().GetFile().SetFileName(name)
#define KRK_LOG_RAW(msg)                                            ::krakoa::EngineLogger::GetLogger().AddRaw(msg)
#define KRK_DBG(msg)                                                ::krakoa::EngineLogger::GetLogger().AddEntry(KRK_LOG_LVL_DBG, ::klib::kLogs::LogMessage(msg))
#define KRK_NRM(msg)                                                ::krakoa::EngineLogger::GetLogger().AddEntry(KRK_LOG_LVL_NRM, ::klib::kLogs::LogMessage(msg))
#define KRK_INF(msg)                                                ::krakoa::EngineLogger::GetLogger().AddEntry(KRK_LOG_LVL_INF, ::klib::kLogs::LogMessage(msg))
#define KRK_WRN(msg)                                                ::krakoa::EngineLogger::GetLogger().AddEntry(KRK_LOG_LVL_WRN, ::klib::kLogs::LogMessage(msg, SOURCE_INFO()))
#define KRK_ERR(msg)                                                ::krakoa::EngineLogger::GetLogger().AddEntry(KRK_LOG_LVL_ERR, ::klib::kLogs::LogMessage(msg, SOURCE_INFO()))
#define KRK_BANNER(msg, descriptor, frontPad, backPad, count)       ::krakoa::EngineLogger::GetLogger().AddBanner(descriptor, msg, frontPad, backPad, count)
#define KRK_SUSPEND()                                               ::krakoa::EngineLogger::GetLogger().GetFile().Close(false)
#define KRK_RESUME()                                                ::krakoa::EngineLogger::GetLogger().GetFile().Open()
#define KRK_LOG_END()                                               ::krakoa::EngineLogger::ShutDown()
#define KRK_LOG_GET_LAST()                                          ::krakoa::EngineLogger::GetLogger().GetLastCachedEntry()
#define KRK_LOG_ERASE_PREV(numOfPrevEntries)                        ::krakoa::EngineLogger::GetLogger().ErasePrevious(numOfPrevEntries)
#define KRK_LOG_CLEAR()                                             ::krakoa::EngineLogger::GetLogger().ClearCache()

