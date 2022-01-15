#pragma once
#include "../UITypes.hpp"

#include "../Flags/SelectableFlags.hpp"

namespace krakoa::ui
{
	bool HandleSelectable(std::string_view label, bool selected, const UICallBack& action);
	bool HandleSelectable(std::string_view label, bool selected, SelectableFlags::Underlying_t flags, const kmaths::Vector2f& size, const UICallBack& action);
	void SetItemDefaultFocused();
}
