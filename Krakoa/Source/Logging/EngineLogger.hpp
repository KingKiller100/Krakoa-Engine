#pragma once

#include <Utility/Logging/kLogProfile.hpp>
#include "../Core/EngineMacros.hpp"

namespace krakoa
{
	using LogProfile = klib::LogProfileRef;

	class KRAKOA_API EngineLogger
	{
	public:
		// Engine side
		static void CoreInit( const std::string_view& initMsg );
		static void ShutDown();

		static LogProfile RegisterProfile( std::string_view name );

		static void SetMinimumLogLevelUsingConfig( klib::LogLevel minLvl );
		static void RemoveIfTooOldFile( std::int64_t maxDays );
		static void RemoveIfTooLarge( std::uint64_t maxBytes );

		static void Raw( std::string_view message );

		static void Trace( std::string_view message );
		static void Debug( std::string_view message );
		static void Info( std::string_view message );
		static void Warn( std::string_view message );
		static void Error( std::string_view message, klib::SourceInfo sourceInfo );

		static void Banner(
			std::string_view message
			, const std::string_view& frontPadding
			, const std::string_view& backPadding
			, std::uint16_t paddingCount
		);
	};
}

// ENGINE SIDE Log Macros
#define KRK_INIT_LOGS(msg)                                          ::krakoa::EngineLogger::CoreInit(msg)
#define KRK_LOG_RAW(msg)                                            ::krakoa::EngineLogger::Raw(msg)
#define KRK_TRC(msg)                                                ::krakoa::EngineLogger::Trace(msg)
#define KRK_DBG(msg)                                                ::krakoa::EngineLogger::Debug(msg)
#define KRK_INF(msg)                                                ::krakoa::EngineLogger::Info(msg)
#define KRK_WRN(msg)                                                ::krakoa::EngineLogger::Warn(msg)
#define KRK_ERR(msg)                                                ::krakoa::EngineLogger::Error(msg, SOURCE_INFO())
#define KRK_BANNER(msg, fPad, bPad, padCount)                       ::krakoa::EngineLogger::Banner(msg, fPad, bPad, padCount)
#define KRK_LOG_END()                                               ::krakoa::EngineLogger::ShutDown()
