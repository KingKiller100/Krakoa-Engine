#pragma once

#include "../EnumHelper/EnumHelper.hpp"

#include <string_view>
#include <cstdint>

namespace klib
{
	namespace kLogs
	{		
		ENUM_CLASS(LogLevel, std::uint8_t,
			BANR, // Debug
			DBUG, // Debug
			NORM, // Normal
			INFO, // Informative
			WARN, // Warning
			ERRR, // Error
			FATL  // Fatal
		);

	}
#ifdef KLIB_SHORTHAND_NAMESPACE
	using namespace kLogs;
#endif

}
