#pragma once

#include "KeyCode.hpp"
#include "MouseButtonCode.hpp"

#include <Maths/Vectors/Vector2.hpp>

namespace krakoa::input
{
	// Keys
	bool IsKeyPressed(KeyCode keycode) noexcept;

	// Mouse
	float GetMousePosX() noexcept;
	float GetMousePosY() noexcept;
	kmaths::Vector2f GetMousePosition() noexcept;
	bool IsMouseButtonPressed(MouseCode mouseCode) noexcept;
}
