#include "Precompile.hpp"
#include "LogOS.hpp"

#include "../../Logging/EngineLogger.hpp"

namespace krakoa::os
{
	void LogOS( const std::string_view& msg )
	{
		KRK_LOG( "Win32", msg.data() );
	}

	void LogOSError( const std::string_view& msg )
	{
		KRK_LOG( "Win32_ERR", msg.data() );
	}
}
