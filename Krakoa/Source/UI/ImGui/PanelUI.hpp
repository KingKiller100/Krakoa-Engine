#pragma once

#include "../UITypes.hpp"
#include "../Flags/WindowFlags.hpp"

namespace krakoa::ui
{
	void DrawPanel(const char* label, const UICallBack& instruction = nullptr);
	void DrawPanel(const char* label, WindowFlags::underlying_t flags, const UICallBack& instruction = nullptr);
	void DrawPanel(const char* label, bool* pOpen, WindowFlags::underlying_t flags, const UICallBack& instruction = nullptr);
}
