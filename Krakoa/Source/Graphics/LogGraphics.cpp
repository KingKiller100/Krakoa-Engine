#include "Precompile.hpp"

#include "LogGraphics.hpp"

#include "../Logging/EngineLogger.hpp"

namespace krakoa::gfx
{
	void LogGFX(const std::string& msg)
	{
		KRK_LOG("GFX", msg);
	}

	void LogGFXError(const std::string& msg)
	{
		KRK_LOG("GFX_ERR", msg);
	}
}
