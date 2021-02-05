#include "Precompile.hpp"
#include "LayoutUI.hpp"

#include <imgui.h>

namespace krakoa::ui
{
	void DrawNewLine(size_t count)
	{
		if (count == 0)
			return;

		for (; count > 0; --count)
		{
			ImGui::NewLine();
		}
	}

	void DrawSameLine(float offsetX, float spacing)
	{
		ImGui::SameLine(offsetX, spacing);
	}

	void DrawSeparator()
	{
		ImGui::Separator();
	}
}
