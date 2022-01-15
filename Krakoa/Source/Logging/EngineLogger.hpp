#pragma once

#include <Utility/Logging/kLogging.hpp>
#include "../Core/EngineMacros.hpp"

namespace krakoa
{
	using LogProfile = klib::LogProfileRef;
	template <typename T>
	using LogDest = klib::LogDispatcher::LogDestRef<T>;

	class KRAKOA_API EngineLogger
	{
	public:
		// Engine side
		static void CoreInit( std::string_view initMsg );
		static void ShutDown();

		static LogProfile RegisterProfile( std::string_view name );

		template <typename T, typename ...Params>
		static LogDest<T> AddDestination( Params&& ...params )
		{
			return dispatcher_.AddDestination<T>( std::forward<Params>( params )... );
		}

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
			, std::string_view frontPadding
			, std::string_view backPadding
			, std::uint16_t paddingCount
		);

	private:
		inline static klib::LogDispatcher dispatcher_;
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
