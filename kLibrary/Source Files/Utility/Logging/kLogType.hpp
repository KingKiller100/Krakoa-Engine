#pragma once

#include "../EnumHelper/EnumHelper.hpp"

#include <string_view>
#include <cstdint>

namespace klib
{
	namespace kLogs
	{		
		ENUM_CLASS(LogLevel, : std::uint16_t,
			DBUG, // Debug
			NORM, // Normal
			INFO, // Informative
			WARN, // Warning
			ERRR, // Error
			FATL  // Fatal
		)
		
		class LogLevelImpl
		{
		public:
			explicit LogLevelImpl(const LogLevel::_enum& lvl);
			LogLevelImpl(const std::string_view& type);
			~LogLevelImpl();

			USE_RESULT std::string_view GetLevel() const;
			
		private:
			std::string type;
		};

	}
}