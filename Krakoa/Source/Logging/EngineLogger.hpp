#pragma once

#include "../Core/EngineMacros.hpp"

namespace krakoa
{
	class KRAKOA_API EngineLogger
	{
	public:
		// Engine side
		static void CoreInit( const std::string_view& initMsg );
		static void ShutDown();

		static void SetMinimumLogLevelUsingConfig( klib::LogLevel minLvl );
		static void RemoveIfTooOldFile( std::int64_t maxDays );
		static void RemoveIfTooLarge( std::uint64_t maxBytes );

		static void Raw( std::string_view message );

		static void Trace( std::string_view profile, std::string_view message );
		static void Debug( std::string_view profile, std::string_view message );
		static void Normal( std::string_view profile, std::string_view message );
		static void Info( std::string_view profile, std::string_view message );
		static void Warn( std::string_view profile, std::string_view message );
		static void Error( std::string_view profile, std::string_view message, klib::SourceInfo sourceInfo );
		
		static void Banner(
			const klib::LogMessage& message
			, const std::string_view& frontPadding
			, const std::string_view& backPadding
			, std::uint16_t paddingCount
		);
	};
}

// ENGINE SIDE Log Macros
#define KRK_INIT_LOGS(msg)                                          ::krakoa::EngineLogger::CoreInit(msg)
#define KRK_LOG_RAW(msg)                                            ::krakoa::EngineLogger::Raw(msg)
#define KRK_TRC(profile, msg)                                       ::krakoa::EngineLogger::Trace(profile, msg)
#define KRK_DBG(profile, msg)                                       ::krakoa::EngineLogger::Debug(profile, msg)
#define KRK_NRM(profile, msg)                                       ::krakoa::EngineLogger::Normal(profile, msg)
#define KRK_INF(profile, msg)                                       ::krakoa::EngineLogger::Info(profile, msg)
#define KRK_WRN(profile, msg)                                       ::krakoa::EngineLogger::Warn(profile, msg)
#define KRK_ERR(profile, msg)                                       ::krakoa::EngineLogger::Error(profile, msg, SOURCE_INFO()))
#define KRK_LOG_END()                                               ::krakoa::EngineLogger::ShutDown()
