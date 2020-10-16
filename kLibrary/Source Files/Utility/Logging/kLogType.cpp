#include "pch.hpp"
#include "kLogType.hpp"

namespace klib
{
	namespace kLogs
	{
		LogLevelImpl::LogLevelImpl(const LogLevel::_enum& lvl)
		{
			type = LogLevel::ToString(lvl);
		}

		LogLevelImpl::LogLevelImpl(const std::string_view& type)
			: type(type)
		{}

		LogLevelImpl::~LogLevelImpl()
		{
		}

		std::string_view LogLevelImpl::GetLevel() const
		{
			return type;
		}
	}
}
