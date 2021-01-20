#include "Precompile.hpp"
#include "Input.hpp"

#include "InputManager.hpp"

namespace krakoa::input
{
	bool IsKeyPressed(KeyCode keycode) noexcept
	{
		return InputManager::IsKeyPressed(keycode);
	}

	float GetMousePosX() noexcept
	{
		return InputManager::GetMousePosX();
	}

	float GetMousePosY() noexcept
	{
		return InputManager::GetMousePosX();
	}

	kmaths::Vector2f GetMousePosition() noexcept
	{
		return InputManager::GetMousePosition();
	}
	
	bool IsMouseButtonPressed(MouseCode mouseCode) noexcept
	{
		return InputManager::IsMouseButtonPressed(mouseCode);
	}
}

