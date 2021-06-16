#include "Precompile.hpp"
#include "LogOS.hpp"

#include "../../Logging/EngineLogger.hpp"

namespace krakoa::os
{
	void LogOS(const std::string& msg)
	{
		KRK_LOG("OS", msg);
	}

	void LogOSError(const std::string& msg)
	{
		KRK_LOG("OS_ERR", msg);
	}
}
