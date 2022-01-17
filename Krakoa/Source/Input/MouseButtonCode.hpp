#pragma once

#include <cstdint>
// Based on GLFW mouse codes

namespace krakoa::input
{
	enum MouseCode : std::uint16_t
	{
		MOUSE_1 = 0,
		MOUSE_2 = 1,
		MOUSE_3 = 2,
		MOUSE_4 = 3,
		MOUSE_5 = 5,
		MOUSE_6 = 6,
		MOUSE_7 = 7,
		MAX_COUNT,
		
		MOUSE_LEFT = MOUSE_1,
		MOUSE_RIGHT = MOUSE_2,
		MOUSE_MIDDLE = MOUSE_3,
	};
}
