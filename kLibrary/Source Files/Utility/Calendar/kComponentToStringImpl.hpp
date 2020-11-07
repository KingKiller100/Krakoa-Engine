#pragma once
#include <functional>

namespace klib::kCalendar
{
	class ComponentToStringImpl
	{
	protected:
		std::string operator()(const std::string_view& format, char token, const std::function<void(size_t, std::string&)>& matchingTokenFunc) const;
	};
}
