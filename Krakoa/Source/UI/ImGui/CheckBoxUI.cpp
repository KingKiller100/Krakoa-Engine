#include "Precompile.hpp"
#include "CheckBoxUI.hpp"

#include <imgui.h>

namespace krakoa::ui
{
	bool DrawCheckBox(const std::string_view& label, bool enabled)
	{
		ImGui::Checkbox(label.data(), &enabled);
		return enabled;
	}
}
