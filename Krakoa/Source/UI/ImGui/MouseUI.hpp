#pragma once

#include "../UITypes.hpp"
#include "../../Input/MouseButtonCode.hpp"

namespace krakoa::ui
{
	namespace popups
	{
		bool DrawMousePopupMenu(const char* id, input::MouseCode mouseButtonCode, bool displayOverItems,
			const UICallBack& callback);
		void DrawMousePopupMenuItem(const char* id, input::MouseCode mouseButtonCode, const UICallBack& callback);
		void DrawMousePopupMenuItem(const char* label, const UICallBack& callback);
	}

	void IsItemClicked(input::MouseCode mouseButtonCode, const UIQueryCallBack& callback);
}