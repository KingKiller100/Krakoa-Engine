#include "Precompile.hpp"
#include "PopupsUI.hpp"

#include <imgui.h>

namespace krakoa::ui::popups
{
	void OpenPopup(const std::string_view& label, WindowFlags::underlying_t flags, const UICallBack& callback)
	{
		ImGui::OpenPopup(label.data());

		if (ImGui::BeginPopup(label.data(), flags))
		{
			callback();
			ImGui::EndPopup();
		}
	}

	void OpenPopup(const std::string_view& label, const UICallBack& callback)
	{
		OpenPopup(label, WindowFlags::None, callback);
	}
}
