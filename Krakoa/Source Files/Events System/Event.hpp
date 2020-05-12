#pragma once

#include "../Core/EngineConfig.hpp"

#include <HelperMacros.hpp>

#include <functional>
#include <ostream>
#include <string>

namespace krakoa::events
{
	enum class EType
	{
		NONE = 0,
		WINDOW_CLOSE, WINDOW_OPEN, WINDOW_RESIZE, WINDOW_FOCUS, WINDOW_MOVED,
		APP_UPDATE, APP_RENDER, APP_TICK,
		KEY_PRESSED, KEY_TYPED, KEY_RELEASED,
		MOUSE_CLICK,   MOUSE_RELEASE, MOUSE_MOVE, MOUSE_SCROLL, MOUSE_BUTTON
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

#define EVENT_CLASS_TYPE(type) static EType GetStaticType() { return EType::##type; }\
								virtual EType GetEventType() const noexcept override { return GetStaticType(); }\
								virtual const char* GetName() const noexcept override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlag() const noexcept override { return category; }

	class KRAKOA_API Event
	{
		friend class EventDispatcher;

	public:
		virtual ~Event() = default;
		
		USE_RESULT virtual EType GetEventType() const noexcept = 0;
		USE_RESULT virtual const char* GetName() const noexcept = 0;
		USE_RESULT virtual int GetCategoryFlag() const noexcept = 0;
		
		USE_RESULT virtual std::string ToString() const noexcept 
		{
			return GetName();
		}

		USE_RESULT bool IsInCategory(ECategory category) const
		{
			return GetCategoryFlag() & category;
		}

		USE_RESULT bool isHandled() const
		{
			return handled;
		}

	protected:
		bool handled = false;
	};

	class EventDispatcher
	{
		template<typename T>
		using EventFunc = std::function<bool(T&)>;

	public:
		EventDispatcher(Event& event)
			: event(event)
		{}

		template<typename T>
		bool Dispatch(EventFunc<T> eFunc)
		{
			if (event.GetEventType() != T::GetStaticType())
				return false;

			event.handled = eFunc(*static_cast<T*>(&event));
			
			return true;
		}

	private:
		Event& event;
	};

	USE_RESULT inline std::ostream& operator<< (std::ostream& os, const Event& ev)
	{
		return os << ev.ToString();
	}
}
