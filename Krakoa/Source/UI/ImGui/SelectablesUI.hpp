#pragma once
#include "../UITypes.hpp"

#include "../Flags/SelectableFlags.hpp"

namespace krakoa::ui
{
	bool HandleSelectable(const std::string_view& label, bool selected, const UICallBack& action);
	bool HandleSelectable(const std::string_view& label, bool selected, SelectableFlags::underlying_t flags, const kmaths::Vector2f& size, const UICallBack& action);
}
