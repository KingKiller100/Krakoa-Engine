#pragma once

#include "../Util/Fmt.hpp"
#include <string>

namespace krakoa::gfx
{
	void LogGFX(const std::string& msg);
	void LogGFXError(const std::string& msg);

	template<typename T, typename ...Ts>
	void LogGFX(const char* fmt, const T& arg, const Ts& ...args)
	{
		LogGFX(util::Fmt(fmt, arg, args...));
	}

	template<typename T, typename ...Ts>
	void LogGFXError(const char* fmt, const T& arg, const Ts& ...args)
	{
		LogGFXError(util::Fmt(fmt, arg, args...));
	}
}
