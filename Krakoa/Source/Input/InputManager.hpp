#pragma once
#include "../Patterns/ManagerBase.hpp"

#include "KeyCode.hpp"
#include "MouseButtonCode.hpp"

#include <Maths/Vectors/Vector2.hpp>

namespace krakoa::input
{
	class InputManager : public patterns::ManagerBase<InputManager>
	{
	public:
		InputManager(Token);

		static void Initialize() noexcept;

		// Keys
		static bool IsKeyPressed(KeyCode keycode) noexcept;
		static bool IsKeyReleased(KeyCode keycode) noexcept;

		// Mouse
		static float GetMousePosX() noexcept;
		static float GetMousePosY() noexcept;
		static kmaths::Vector2f GetMousePosition() noexcept;
		static bool IsMouseButtonPressed(MouseCode button) noexcept;

	protected:
		static void CreateImpl() noexcept;

		// Keys
		USE_RESULT virtual bool IsKeyPressedImpl(KeyCode keycode) const noexcept = 0;
		USE_RESULT virtual bool IsKeyReleasedImpl(KeyCode keycode) const noexcept = 0;

		// Mouse
		USE_RESULT virtual float GetMousePosXImpl() const noexcept = 0;
		USE_RESULT virtual float GetMousePosYImpl() const noexcept = 0;
		USE_RESULT virtual kmaths::Vector2f GetMousePositionImpl() const noexcept = 0;
		USE_RESULT virtual bool IsMouseButtonPressedImpl(const MouseCode button) const noexcept = 0;

	};
}