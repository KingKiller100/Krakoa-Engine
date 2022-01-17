#include "Precompile.hpp"
#include "InputManager.hpp"

namespace krakoa::input
{
	InputManager::InputManager(Token)
	{}

	void InputManager::Initialize( std::shared_ptr<iWindow> window) noexcept
	{
		CreateImpl();
		window_ = window;
	}

	bool InputManager::IsKeyPressed(KeyCode keycode) noexcept
	{
		return Reference().IsKeyPressedImpl(keycode);
	}

	bool InputManager::IsKeyReleased(KeyCode keycode) noexcept
	{
		return Reference().IsKeyReleasedImpl(keycode);
	}

	float InputManager::GetMousePosX() noexcept
	{
		return Reference().GetMousePosXImpl();
	}

	float InputManager::GetMousePosY() noexcept
	{
		return Reference().GetMousePosYImpl();
	}

	maths::Point InputManager::GetMousePosition() noexcept
	{
		return Reference().GetMousePositionImpl();
	}

	bool InputManager::IsMouseButtonPressed(MouseCode button) noexcept
	{
		return Reference().IsMouseButtonPressedImpl(button);
	}

	iWindow& InputManager::GetWindow()
	{
		return *window_;
	}
}
