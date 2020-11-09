#pragma once
#include <functional>

#include "../../HelperMacros.hpp"

namespace klib::kCalendar
{
	class CalendarComponentToStringImpl
	{
	protected:
		void ToStringImpl(const std::string_view& format, char token
			, const std::function<void(char)>& noMatchingTokenFunc
			, const std::function<void(size_t)>& matchingTokenFunc) const;

		USE_RESULT virtual std::string ToStringUsingTokenCount(const size_t count) const = 0;
	};
}
