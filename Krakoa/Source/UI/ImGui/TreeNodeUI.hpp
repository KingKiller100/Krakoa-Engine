#pragma once

#include "../UITypes.hpp"
#include "../Flags/TreeNodeFlags.hpp"

namespace krakoa::ui
{
	bool DrawTreeNode(const std::string_view& label, void* id, TreeNodeFlags::underlying_t flags, const UICallBack& callback);
	bool DrawTreeNode(const std::string_view& label, void* id, TreeNodeFlags::underlying_t flags, const UIQueryCallBack& callback);
}
