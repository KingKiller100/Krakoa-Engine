#pragma once

#include "../../Util/Fmt.hpp"

#include <string>

namespace krakoa::os
{
	void LogOS(const std::string& msg);
	void LogOSError(const std::string& msg);

	template<typename T, typename ...Ts>
	void LogOS(const char* fmt, const T& arg, const Ts& ...args)
	{
		LogOS(util::Fmt(fmt, arg, args...));
	}
	
	template<typename T, typename ...Ts>
	void LogOSError(const char* fmt, const T& arg, const Ts& ...args)
	{
		LogOSError(util::Fmt(fmt, arg, args...));
	}
}
