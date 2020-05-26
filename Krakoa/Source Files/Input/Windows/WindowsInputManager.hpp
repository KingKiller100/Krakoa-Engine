#pragma once
#include "../InputManager.hpp"

namespace krakoa::input
{
	class WindowsInputManager final : public InputManager
	{
	public:
		WindowsInputManager(Token&&) noexcept;
		~WindowsInputManager();

	protected:
		bool IsKeyPressedImpl(const int keycode) const noexcept override;

		bool IsMouseButtonPressedImpl(const MouseInputType button) const noexcept override;
		std::pair<float, float> GetMousePositionImpl() const noexcept override;
		float GetMousePosXImpl() const noexcept override;
		float GetMousePosYImpl() const noexcept override;
	};
}