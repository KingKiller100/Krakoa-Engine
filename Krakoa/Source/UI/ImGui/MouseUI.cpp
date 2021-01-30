#include "Precompile.hpp"
#include "MouseUI.hpp"

#include "MenuUI.hpp"

#include <imgui.h>

namespace krakoa::ui
{
	bool DrawMousePopupMenu(const char* id, input::MouseCode mouseButtonCode, bool displayOverItems,
		const UICallBack& callback)
	{
		if (ImGui::BeginPopupContextWindow(id, mouseButtonCode, displayOverItems))
		{
			callback();
			ImGui::EndPopup();
			return true;
		}
		return false;
	}

	void DrawMousePopupMenuItem(const char* id, input::MouseCode mouseButtonCode, const UICallBack& callback)
	{
		if (ImGui::BeginPopupContextItem(id, mouseButtonCode))
		{
			callback();
			ImGui::EndPopup();
		}
	}

	void DrawMousePopupMenuItem(const char* label, const UICallBack& callback)
	{
		DrawMenuItem(label, callback);
	}

	void IsItemClicked(input::MouseCode mouseButtonCode, const UIQueryCallBack& callback)
	{
		const bool clicked = ImGui::IsItemClicked(mouseButtonCode);
		callback(clicked);
	}
}
