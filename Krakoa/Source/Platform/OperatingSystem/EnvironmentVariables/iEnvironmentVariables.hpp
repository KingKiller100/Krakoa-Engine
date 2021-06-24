#pragma once

#include <string>

namespace krakoa::os
{
	template <typename T>
	class iEnvironmentVariables
	{
	public:
		virtual ~iEnvironmentVariables() = default;

		USE_RESULT virtual std::basic_string<T> GetVariable(const std::basic_string<T>& varKey) const = 0;
		USE_RESULT virtual std::vector<std::basic_string_view<T>> GetKeys() const = 0;
	};
}

