#include "Precompile.hpp"
#include "PanelUI.hpp"

#include <imgui.h>

namespace krakoa::ui
{
	void DrawPanel(const char* label, const UICallBack& instruction)
	{
		DrawPanel(label, WindowFlags::None, instruction);
	}

	void DrawPanel(const char* label, WindowFlags::underlying_t flags, const UICallBack& instruction)
	{
		DrawPanel(label, nullptr, flags, instruction);
	}

	void DrawPanel(const char* label, bool* pOpen, WindowFlags::underlying_t flags, const UICallBack& instruction)
	{
		ImGui::Begin(label, pOpen, flags);
		if (instruction)
			instruction();
		ImGui::End();
	}
}
