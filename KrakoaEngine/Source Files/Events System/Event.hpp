#pragma once

#include <Core/EngineCore.hpp>

#include <string>

namespace krakoa::events
{
	enum class EType
	{
		NONE = 0,
		WINDOW_CLOSE, WINDOW_OPEN, WINDOW_RESIZE, WINDOW_FOCUS, WINDOW_MOVED,
		APP_UPDATE, APP_RENDER,
		KEY_PRESSED, KEY_RELEASED,
		LEFT_BUTTON_MOUSE_CLICK,   LEFT_BUTTON_MOUSE_RELEASE,
		RIGHT_BUTTON_MOUSE_CLICK,  RIGHT_BUTTON_MOUSE_RELEASE,
		MOUSE_MOVED, MOUSE_SCROLL, MOUSE_BUTTON_EVENT
	};

	enum ECategory
	{
		NONE = 0, 
		E_C_APP           = BIT_SHIFT(0),
		E_C_INPUT         = BIT_SHIFT(1),
		E_C_KEYBOARD      = BIT_SHIFT(2),
		E_C_MOUSE         = BIT_SHIFT(3),
		E_C_MOUSE_BUTTON  = BIT_SHIFT(4),
	};



	class KRAKOA_API Event
	{
		friend class EventDispacher;

	public:
		virtual EType GetEventType() const noexcept = 0;
		virtual const char* GetName() const noexcept = 0;
		virtual int GetCategoryFlag() const noexcept = 0;
		virtual std::string_view ToString() const
		{
			return GetName();
		}
	};
}