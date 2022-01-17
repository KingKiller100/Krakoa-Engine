#pragma once
#include "../InputManager.hpp"
#include "../../EventsSystem/ApplicationEvent.hpp"

namespace krakoa::input
{
	class WindowsInputManager final : public InputManager
	{
	public:
		WindowsInputManager(const Token& t) noexcept;
		~WindowsInputManager();

	protected:
		USE_RESULT bool IsKeyPressedImpl(KeyCode keycode) const noexcept override;
		USE_RESULT bool IsKeyReleasedImpl(KeyCode keycode) const noexcept override;

		USE_RESULT bool IsMouseButtonPressedImpl(MouseCode button) const noexcept override;
		USE_RESULT maths::Point GetMousePositionImpl() const noexcept override;
		USE_RESULT float GetMousePosXImpl() const noexcept override;
		USE_RESULT float GetMousePosYImpl() const noexcept override;
	};
}
