#include "pch.hpp"
#include "kLogLevelImpl.hpp"
#include "kLogLevel.hpp"

namespace klib
{
	namespace kLogs
	{
		LogLevelImpl::LogLevelImpl(const LogLevel& lvl)
		{
			type = lvl.ToString();
		}

		LogLevelImpl::LogLevelImpl(const std::string_view& type, const std::string_view frontPad,
			const std::string_view rearPad)
			: type(type)
			, frontPadding(frontPad)
			, rearPadding(rearPad)
		{
		}
	}
}
