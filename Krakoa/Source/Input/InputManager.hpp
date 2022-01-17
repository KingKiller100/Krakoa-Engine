#pragma once
#include "../Core/iWindow.hpp"
#include "../Patterns/ManagerBase.hpp"
#include "../Maths/Point.hpp"

#include "KeyCode.hpp"
#include "MouseButtonCode.hpp"

namespace krakoa::input
{
	class InputManager : public patterns::ManagerBase<InputManager>
	{
	public:
		InputManager(Token);

		static void Initialize(std::shared_ptr<iWindow> window) noexcept;

		// Keys
		static bool IsKeyPressed(KeyCode keycode) noexcept;
		static bool IsKeyReleased(KeyCode keycode) noexcept;

		// Mouse
		static float GetMousePosX() noexcept;
		static float GetMousePosY() noexcept;
		static maths::Point GetMousePosition() noexcept;
		static bool IsMouseButtonPressed(MouseCode button) noexcept;

	protected:
		static void CreateImpl() noexcept;

		static iWindow& GetWindow();

		// Keys
		USE_RESULT virtual bool IsKeyPressedImpl(KeyCode keycode) const noexcept = 0;
		USE_RESULT virtual bool IsKeyReleasedImpl(KeyCode keycode) const noexcept = 0;

		// Mouse
		USE_RESULT virtual float GetMousePosXImpl() const noexcept = 0;
		USE_RESULT virtual float GetMousePosYImpl() const noexcept = 0;
		USE_RESULT virtual maths::Point GetMousePositionImpl() const noexcept = 0;
		USE_RESULT virtual bool IsMouseButtonPressedImpl(const MouseCode button) const noexcept = 0;

	private:
		static std::shared_ptr<iWindow> window_;
	};
}