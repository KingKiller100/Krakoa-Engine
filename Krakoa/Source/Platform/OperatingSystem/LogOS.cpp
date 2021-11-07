#include "Precompile.hpp"
#include "LogOS.hpp"

#include "../../Logging/EngineLogger.hpp"

namespace krakoa::os
{
	void LogOS(const std::string_view& msg)
	{
		KRK_LOG("WIN32", msg.data());
	}

	void LogOSError(const std::string_view& msg)
	{
		KRK_LOG("WIN32_ERR", msg.data());
	}
}
