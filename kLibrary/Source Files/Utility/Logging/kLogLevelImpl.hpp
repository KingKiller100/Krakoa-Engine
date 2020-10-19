#pragma once
#include "../../HelperMacros.hpp"
#include <string>

namespace klib
{
	namespace kLogs
	{
		class LogLevel;

		class LogLevelImpl
		{
		public:
			explicit LogLevelImpl(const LogLevel& lvl);
			LogLevelImpl(const std::string_view& type);

			USE_RESULT std::string_view GetLevel() const;

		private:
			std::string type;
		};
	}

#ifdef KLIB_SHORTHAND_NAMESPACE
	using namespace kLogs;
#endif
}
