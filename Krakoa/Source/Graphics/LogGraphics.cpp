#include "Precompile.hpp"

#include "LogGraphics.hpp"

#include "../Logging/EngineLogger.hpp"

namespace krakoa::gfx
{
	LogProfile g_GfxLog = EngineLogger::RegisterProfile( "GFX" );

	void LogGFX( const std::string& msg )
	{
		g_GfxLog->AddEntry( klib::LogLevel::INF, msg );
	}

	void LogGFXError( const std::string& msg )
	{
		g_GfxLog->AddEntry( klib::LogLevel::ERR, msg );
	}
}
