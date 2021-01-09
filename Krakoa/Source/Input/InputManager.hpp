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
		InputManager(Token) {}
		
		static void Initialize() noexcept                                             { CreateImpl(); }

		// Keys
		static inline bool IsKeyPressed(KeyCode keycode) noexcept                   { return Reference().IsKeyPressedImpl(keycode); }
		
		// Mouse
		static inline float GetMousePosX() noexcept                                   { return Reference().GetMousePosXImpl(); }
		static inline float GetMousePosY() noexcept                                   { return Reference().GetMousePosYImpl(); }
		static inline kmaths::Vector2f GetMousePosition() noexcept                    { return Reference().GetMousePositionImpl(); }
		static inline bool IsMouseButtonPressed(MouseCode button) noexcept       { return Reference().IsMouseButtonPressedImpl(button); }

	protected:
		static void CreateImpl() noexcept;

		// Keys
		USE_RESULT virtual bool IsKeyPressedImpl(KeyCode keycode) const noexcept = 0;

		// Mouse
		USE_RESULT virtual float GetMousePosXImpl() const noexcept = 0;
		USE_RESULT virtual float GetMousePosYImpl() const noexcept = 0;
		USE_RESULT virtual kmaths::Vector2f GetMousePositionImpl() const noexcept = 0;
		USE_RESULT virtual bool IsMouseButtonPressedImpl(const MouseCode button) const noexcept = 0;

	};
}