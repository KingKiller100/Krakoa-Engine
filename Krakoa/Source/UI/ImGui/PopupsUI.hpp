#pragma once

#include "../UITypes.hpp"

#include "../Flags/WindowFlags.hpp"

#include <string>

namespace krakoa::ui::popups
{
	void OpenPopup(const std::string_view& label, WindowFlags::underlying_t flags, const UICallBack& callback);
	void OpenPopup(const std::string_view& label, const UICallBack& callback);
}
