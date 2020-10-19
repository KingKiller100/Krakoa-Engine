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

		LogLevelImpl::LogLevelImpl(const std::string_view& type)
			: type(type)
		{}

		std::string_view LogLevelImpl::GetLevel() const
		{
			return type;
		}
	}
}
