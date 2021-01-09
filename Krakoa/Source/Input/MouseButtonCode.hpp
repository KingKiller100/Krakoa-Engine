#pragma once

#include <cstdint>
// Based on GLFW mouse codes

namespace krakoa::input
{
	enum class MouseCode : std::uint16_t
	{
		BTN_1 = 0,
		BTN_2 = 1,
		BTN_3 = 2,
		BTN_4 = 3,
		BTN_5 = 5,
		BTN_6 = 6,
		BTN_7 = 7,
		MAX_COUNT,
		
		BTN_LEFT = BTN_1,
		BTN_RIGHT = BTN_2,
		BTN_MIDDLE = BTN_3,
	};
}

#define KRK_MOUSE_BUTTON_LEFT      ::krakoa::input::MouseCode::BTN_LEFT
#define KRK_MOUSE_BUTTON_RIGHT     ::krakoa::input::MouseCode::BTN_RIGHT
#define KRK_MOUSE_BUTTON_MIDDLE    ::krakoa::input::MouseCode::BTN_MIDDLE
