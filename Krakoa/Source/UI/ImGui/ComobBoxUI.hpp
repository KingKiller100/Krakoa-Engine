#pragma once
#include "../UITypes.hpp"

#include "../Flags/ComboBoxFlags.hpp"


namespace krakoa::ui
{
	bool DrawComboBox(std::string_view label, const char* previewSelection, ComboBoxFlags::Underlying_t flags, const UICallBack& action);
}
