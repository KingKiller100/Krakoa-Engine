#pragma once
#include "../InputManager.hpp"
#include "../../Events System/ApplicationEvent.hpp"
#include "../../Events System/ApplicationEvent.hpp"

namespace krakoa::input
{
	class WindowsInputManager final : public InputManager
	{
	public:
		WindowsInputManager(const Token& t) noexcept;
		~WindowsInputManager();

	protected:
		USE_RESULT bool IsKeyPressedImpl(const int keycode) const noexcept override;

		USE_RESULT bool IsMouseButtonPressedImpl(const MouseInputType button) const noexcept override;
		USE_RESULT kmaths::Vector2f GetMousePositionImpl() const noexcept override;
		USE_RESULT float GetMousePosXImpl() const noexcept override;
		USE_RESULT float GetMousePosYImpl() const noexcept override;
	};
}
