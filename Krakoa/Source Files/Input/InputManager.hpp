#pragma once
#include "../Patterns/SimpleSingleton.hpp"
#include "../Patterns/ManagerBase.hpp"


namespace krakoa::input
{
	enum class MouseInputType : int
	{
		LEFT,
		RIGHT,
		MIDDLE
	};

	class InputManager : public patterns::ManagerBase<InputManager>
	{
	public:
		static void Initialize() noexcept                                             { CreateImpl(); }

		// Keys
		static inline bool IsKeyPressed(const int keycode) noexcept                   { return Reference().IsKeyPressedImpl(keycode); }
		
		// Mouse
		static inline float GetMousePosX() noexcept                                   { return Reference().GetMousePosXImpl(); }
		static inline float GetMousePosY() noexcept                                   { return Reference().GetMousePosYImpl(); }
		static inline std::pair<float, float> GetMousePosition() noexcept             { return Reference().GetMousePositionImpl(); }
		static inline bool IsMouseButtonPressed(MouseInputType button) noexcept       { return Reference().IsMouseButtonPressedImpl(button); }

	protected:
		static void CreateImpl() noexcept;

		// Keys
		virtual bool IsKeyPressedImpl(const int keycode) const noexcept = 0;

		// Mouse
		virtual float GetMousePosXImpl() const noexcept = 0;
		virtual float GetMousePosYImpl() const noexcept = 0;
		virtual std::pair<float, float> GetMousePositionImpl() const noexcept = 0;
		virtual bool IsMouseButtonPressedImpl(const MouseInputType button) const noexcept = 0;

	};
}