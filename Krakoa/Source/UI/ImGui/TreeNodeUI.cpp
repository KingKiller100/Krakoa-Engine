#include "Precompile.hpp"
#include "TreeNodeUI.hpp"

#include <imgui.h>

namespace krakoa::ui
{
	bool DrawTreeNode(std::string_view label, void* id, TreeNodeFlags::Underlying_t flags,
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

	bool DrawTreeNode(std::string_view label, void* id, TreeNodeFlags::Underlying_t flags,
		const UIQueryCallBack& callback)
	{
		const bool opened = ImGui::TreeNodeEx(id, flags, "%s", label.data());
		callback(opened);
		if (opened)
			ImGui::TreePop();
		return opened;
	}
}
