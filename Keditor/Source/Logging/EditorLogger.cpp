#include "EditorLogger.hpp"

#include <Logging/EngineLogger.hpp>

namespace krakoa
{
	LogProfile g_EditorLog = EngineLogger::RegisterProfile( "Keditor" );

	void LogEditorTrace( const std::string_view& msg )
	{
		g_EditorLog->AddEntry( klib::LogLevel::TRC, msg );
	}

	void LogEditorDebug( const std::string_view& msg )
	{
		g_EditorLog->AddEntry( klib::LogLevel::DBG, msg );
	}

	void LogEditorInfo( const std::string_view& msg )
	{
		g_EditorLog->AddEntry( klib::LogLevel::INF, msg );
	}

	void LogEditorWarn( const std::string_view& msg )
	{
		g_EditorLog->AddEntry( klib::LogLevel::WRN, msg );
	}

	void LogEditorError( const std::string_view& msg )
	{
		g_EditorLog->AddEntry( klib::LogLevel::ERR, msg );
	}
}
