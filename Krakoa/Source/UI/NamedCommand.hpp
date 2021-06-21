#pragma once

#include <string>
#include <functional>

namespace krakoa::ui
{
	struct NamedCommand
	{
		std::string label;
		std::function<void()> callback;
	};
}
