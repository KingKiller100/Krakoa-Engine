#include "Precompile.hpp"
#include "TextUI.hpp"

#include "imgui.h"

namespace krakoa::ui
{
	void DrawRawText(std::string_view text)
	{
		ImGui::Text(text.data());
	}

	bool DrawInputTextBox(std::string_view label, char* buf, size_t buf_size, InputTextFlags::Underlying_t flags,
		const UICallBack& cb)
	{
		if (ImGui::InputText(label.data(), buf, buf_size, flags))
		{
			cb();
			return true;
		}
		return false;
	}
}
