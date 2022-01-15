#pragma once

#include "../UITypes.hpp"

#include "../Flags/WindowFlags.hpp"

#include <string>

namespace krakoa::ui::popups
{
	void OpenPopup(std::string_view label);
	void DrawPopup(std::string_view label, WindowFlags::Underlying_t flags, const UICallBack& callback);
	void DrawPopup(std::string_view label, const UICallBack& callback);
	void DrawPopupOption(std::string_view label, const UICallBack& callback);
}
