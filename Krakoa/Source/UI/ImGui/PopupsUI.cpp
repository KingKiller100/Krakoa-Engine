#include "Precompile.hpp"
#include "PopupsUI.hpp"

#include <imgui.h>

#include "MenuUI.hpp"

namespace krakoa::ui::popups
{
	void OpenPopup(std::string_view label)
	{
		ImGui::OpenPopup(label.data());
	}

	void DrawPopup(std::string_view label, WindowFlags::Underlying_t flags, const UICallBack& callback)
	{
		if (ImGui::BeginPopup(label.data(), flags))
		{
			callback();
			ImGui::EndPopup();
		}
	}

	void DrawPopup(std::string_view label, const UICallBack& callback)
	{
		DrawPopup(label, WindowFlags::None, callback);
	}

	void DrawPopupOption(std::string_view label, const UICallBack& callback)
	{
		DrawMenuItem(label, [&]()
			{
				callback();
				ImGui::CloseCurrentPopup();
			});
	}
}
