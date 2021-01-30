#include "Precompile.hpp"
#include "SelectablesUI.hpp"
#include <Maths/Vectors/Vector2.hpp>

#include <imgui.h>

namespace krakoa::ui
{
	bool HandleSelectable(const std::string_view& label, bool selected, const UICallBack& action)
	{
		return HandleSelectable(label, selected, SelectableFlags::None, kmaths::Vector2f(), action);
	}

	bool HandleSelectable(const std::string_view& label, bool selected, SelectableFlags::underlying_t flags,
		const kmaths::Vector2f& size, const UICallBack& action)
	{
		if (ImGui::Selectable(label.data(), selected, flags, { size.x, size.y }))
		{
			action();
			return true;
		}

		return false;
	}
}
