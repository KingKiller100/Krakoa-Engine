#pragma once

#include <functional>

namespace krakoa::ui
{
	using UICallBack = std::function<void()>;
	using UIQueryCallBack = std::function<void(bool)>;
}
