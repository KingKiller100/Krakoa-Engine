#pragma once

#include "../UITypes.hpp"

#include <string>

namespace krakoa::ui
{
	void DrawMenuBar(const UICallBack& content);

	void DrawMenu(const std::string_view& label, const UICallBack& content);
	void DrawMenu(const std::string_view& label, bool enabled, const UICallBack& content);

	void DrawMenuItem(const std::string_view& label, const UICallBack& callback);
	void DrawMenuItem(const std::string_view& label, bool selected, const UICallBack& callback);
	void DrawMenuItem(const std::string_view& label, bool selected, bool enabled, const UICallBack& callback);

	void DrawMenuItem(const std::string_view& label, const std::string_view& shortcut, const UICallBack& callback);
	void DrawMenuItem(const std::string_view& label, const std::string_view& shortcut, bool selected, const UICallBack& callback);
	void DrawMenuItem(const std::string_view& label, const std::string_view& shortcut, bool selected, bool enabled, const UICallBack& callback);
}