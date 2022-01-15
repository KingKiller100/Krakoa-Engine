#pragma once

#include "../UITypes.hpp"

#include <string>

namespace krakoa::ui
{
	void DrawMenuBar(const UICallBack& content);

	void DrawMenu(std::string_view label, const UICallBack& content);
	void DrawMenu(std::string_view label, bool enabled, const UICallBack& content);

	void DrawMenuItem(std::string_view label, const UICallBack& callback);
	void DrawMenuItem(std::string_view label, bool selected, const UICallBack& callback);
	void DrawMenuItem(std::string_view label, bool selected, bool enabled, const UICallBack& callback);

	void DrawMenuItem(std::string_view label, std::string_view shortcut, const UICallBack& callback);
	void DrawMenuItem(std::string_view label, std::string_view shortcut, bool selected, const UICallBack& callback);
	void DrawMenuItem(std::string_view label, std::string_view shortcut, bool selected, bool enabled, const UICallBack& callback);
}