#include "Precompile.hpp"
#include "InputManager.hpp"

namespace krakoa::input
{
	InputManager::InputManager(Token)
	{}

	void InputManager::Initialize() noexcept
	{
		CreateImpl();
	}

	bool InputManager::IsKeyPressed(KeyCode keycode) noexcept
	{
		return Reference().IsKeyPressedImpl(keycode);
	}

	float InputManager::GetMousePosX() noexcept
	{
		return Reference().GetMousePosXImpl();
	}

	float InputManager::GetMousePosY() noexcept
	{
		return Reference().GetMousePosYImpl();
	}

	kmaths::Vector2f InputManager::GetMousePosition() noexcept
	{
		return Reference().GetMousePositionImpl();
	}

	bool InputManager::IsMouseButtonPressed(MouseCode button) noexcept
	{
		return Reference().IsMouseButtonPressedImpl(button);
	}
}