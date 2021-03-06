﻿#pragma once
#include "../UITypes.hpp"

#include "../Flags/ComboBoxFlags.hpp"


namespace krakoa::ui
{
	bool DrawComboBox(const std::string_view& label, const char* previewSelection, ComboBoxFlags::underlying_t flags, const UICallBack& action);
}
