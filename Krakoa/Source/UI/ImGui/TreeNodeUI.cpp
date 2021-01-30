#include "Precompile.hpp"
#include "TreeNodeUI.hpp"

#include <imgui.h>

namespace krakoa::ui
{
	bool DrawTreeNode(const std::string_view& label, void* id, TreeNodeFlags::underlying_t flags,
		const UICallBack& callback)
	{
		const bool opened = ImGui::TreeNodeEx(id, flags, "%s", label.data());
		if (opened)
		{
			if (callback)
				callback();
			ImGui::TreePop();
		}
		return opened;
	}
}
