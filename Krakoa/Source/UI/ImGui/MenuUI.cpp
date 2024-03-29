﻿#include "Precompile.hpp"
#include "MenuUI.hpp"

#include <imgui.h>

namespace krakoa::ui
{
	void DrawMenuBar(const UICallBack& content)
	{
		if (ImGui::BeginMenuBar())
		{
			content();
			ImGui::EndMenuBar();
		}
	}

	void DrawMenu(std::string_view label, bool enabled, const UICallBack& content)
	{
		if (ImGui::BeginMenu(label.data(), enabled))
		{
			content();
			ImGui::EndMenu();
		}
	}

	void DrawMenu(std::string_view label, const UICallBack& content)
	{
		DrawMenu(label, true, content);
	}

	void DrawMenuItem(std::string_view label, bool selected, bool enabled, const UICallBack& callback)
	{
		if (ImGui::MenuItem(label.data(), nullptr, selected, enabled))
		{
			callback();
		}
	}

	void DrawMenuItem(std::string_view label, bool selected, const UICallBack& callback)
	{
		DrawMenuItem(label, selected, true, callback);
	}

	void DrawMenuItem(std::string_view label, const UICallBack& callback)
	{
		DrawMenuItem(label, false, true, callback);
	}

	void DrawMenuItem(std::string_view label, std::string_view shortcut, const UICallBack& callback)
	{
		DrawMenuItem(label, shortcut, false, callback);
	}

	void DrawMenuItem(std::string_view label, std::string_view shortcut, bool selected,
		const UICallBack& callback)
	{
		DrawMenuItem(label, shortcut, selected, true, callback);
	}

	void DrawMenuItem(std::string_view label, std::string_view shortcut, bool selected, bool enabled, const UICallBack& callback)
	{
		if (ImGui::MenuItem(label.data(), shortcut.data(), selected, enabled))
		{
			callback();
		}

	}

}
