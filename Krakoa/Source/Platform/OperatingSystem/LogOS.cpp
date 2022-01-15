#include "Precompile.hpp"
#include "LogOS.hpp"

#include "../../Logging/EngineLogger.hpp"

namespace krakoa::os
{
	LogProfile g_OsLog = EngineLogger::RegisterProfile( "Win32" );

	void LogOS( std::string_view msg )
	{
		g_OsLog->AddEntry(klib::LogLevel::INF, msg);
	}

	void LogOSError( std::string_view msg )
	{
		g_OsLog->AddEntry(klib::LogLevel::ERR, msg);
	}
}
