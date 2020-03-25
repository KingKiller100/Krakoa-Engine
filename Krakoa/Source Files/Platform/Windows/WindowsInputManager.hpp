#include "Input Manager/InputManager.hpp"

namespace krakoa::input
{
	class WindowsInputManager : public InputManager
	{
	public:
		WindowsInputManager(Token&& t);
		~WindowsInputManager();

	protected:
		//void CreateImpl() noexcept;

		bool IsKeyPressedImpl(const int keycode) const noexcept override;

		bool IsMouseButtonPressedImpl(const MouseButtonType button) const noexcept override;
		std::pair<float, float> GetMousePositionImpl() const noexcept override;
		float GetMousePosXImpl() const noexcept override;
		float GetMousePosYImpl() const noexcept override;

	};
}