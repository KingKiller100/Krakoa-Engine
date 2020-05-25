#pragma once
#include "../Patterns/SimpleSingleton.hpp"


namespace krakoa::input
{
	enum class MouseButtonType : int
	{
		LEFT,
		RIGHT,
		MIDDLE
	};

	class InputManager : protected patterns::SimpleSingleton<InputManager>
	{
	public:
		static void Initialize() noexcept                                            { CreateImpl(); }

		// Keys
		static inline bool IsKeyPressed(const int keycode) noexcept                   { return Reference().IsKeyPressedImpl(keycode); }
		
		// Mouse
		static inline float GetMousePosX() noexcept                                   { return Reference().GetMousePosXImpl(); }
		static inline float GetMousePosY() noexcept                                   { return Reference().GetMousePosYImpl(); }
		static inline std::pair<float, float> GetMousePosition() noexcept             { return Reference().GetMousePositionImpl(); }
		static inline bool IsMouseButtonPressed(MouseButtonType button) noexcept      { return Reference().IsMouseButtonPressedImpl(button); }

	protected:
		static void CreateImpl() noexcept;

		// Keys
		virtual bool IsKeyPressedImpl(const int keycode) const noexcept = 0;

		// Mouse
		virtual float GetMousePosXImpl() const noexcept = 0;
		virtual float GetMousePosYImpl() const noexcept = 0;
		virtual std::pair<float, float> GetMousePositionImpl() const noexcept = 0;
		virtual bool IsMouseButtonPressedImpl(const MouseButtonType button) const noexcept = 0;
	};
}