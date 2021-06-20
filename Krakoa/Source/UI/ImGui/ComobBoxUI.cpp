#include "Precompile.hpp"
#include "ComobBoxUI.hpp"

#include <imgui.h>


namespace krakoa::ui
{

	bool DrawComboBox(const std::string_view& label, const char* previewSelection, ComboBoxFlags::Underlying_t flags,
		const UICallBack& action)
	{
		if (ImGui::BeginCombo(label.data(), previewSelection, flags))
		{
			if (action)
				action();
			ImGui::EndCombo();
			return true;
		}
		return false;
	}
}
