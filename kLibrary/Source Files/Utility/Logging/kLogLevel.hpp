#pragma once

#include "../EnumHelper/EnumHelper.hpp"

#include <string_view>
#include <cstdint>

namespace klib
{
	namespace kLogs
	{		
		ENUM_CLASS(LogLevel, std::uint8_t,
			DBUG, // Debug
			NORM, // Normal
			INFO, // Informative
			WARN, // Warning
			ERRR, // Error
			FATL,  // Fatal
			BANR // Banner
		);

	}
#ifdef KLIB_SHORTHAND_NAMESPACE
	using namespace kLogs;
#endif

}
