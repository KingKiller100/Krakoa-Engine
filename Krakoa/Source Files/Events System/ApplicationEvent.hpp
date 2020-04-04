#pragma once

#include "Event.hpp"

#include <Maths/Vectors/Vector2.hpp>
#include <Utility/Format/kFormatToString.hpp>

#include <string>

namespace krakoa::events
{
	EXPIMP_TEMPLATE template struct KRAKOA_API kmaths::Vector<2, float>;
	EXPIMP_TEMPLATE template struct KRAKOA_API kmaths::Vector<2, int>;

	using namespace kmaths;
	using namespace klib;


	class KRAKOA_API ApplicationEvent : public Event
	{
	public:
		EVENT_CLASS_CATEGORY(E_C_APP)

	protected:
		ApplicationEvent() = default;
	};

	class KRAKOA_API WindowResizeEvent : public ApplicationEvent
	{
	public:
		WindowResizeEvent(const Vector2f& dimensions)
			: dimensions(dimensions)
		{}

		WindowResizeEvent(const float width, const float height)
			: dimensions(width, height)
		{}

		USE_RESULT float GetWidth() const noexcept
		{
			return dimensions.X();
		}
		USE_RESULT float GetHeight() const noexcept
		{
			return dimensions.Y();
		}
		USE_RESULT Vector2f& GetDimensions() noexcept
		{
			return dimensions;
		}

		USE_RESULT std::string ToString() const noexcept override
		{
			return kFormat::ToString("Window Resize Event: (%.2f, %.2f)", GetWidth(), GetHeight());
		}

		EVENT_CLASS_TYPE(WINDOW_RESIZE)
	private:
		Vector2f dimensions;
	};

	class KRAKOA_API WindowOpenEvent : public ApplicationEvent
	{
	public:
		WindowOpenEvent() = default;

		USE_RESULT std::string ToString() const noexcept override
		{
			return "Window Open Event!";
		}

		EVENT_CLASS_TYPE(WINDOW_OPEN)
	};

	class KRAKOA_API WindowFocusEvent : public ApplicationEvent
	{
	public:
		WindowFocusEvent() = default;

		USE_RESULT std::string ToString() const noexcept override
		{
			return "Window Focus Event!";
		}

		EVENT_CLASS_TYPE(WINDOW_FOCUS)
	};


	class KRAKOA_API WindowMovedEvent : public ApplicationEvent
	{
	public:
		WindowMovedEvent(const Vector2f& newPos)
			: newPos(newPos)
		{}

		WindowMovedEvent(const float x, const float y)
			: newPos(x, y)
		{}


		USE_RESULT float GetX() const noexcept
		{
			return newPos.X();
		}
		USE_RESULT float GetY() const noexcept
		{
			return newPos.Y();
		}
		USE_RESULT Vector2f& GetDimensions() noexcept
		{
			return newPos;
		}
		USE_RESULT std::string ToString() const noexcept override
		{
			return kFormat::ToString("Window Moved Event: New Position - (%.2f, %.2f)", GetX(), GetY());
		}

		EVENT_CLASS_TYPE(WINDOW_MOVED)

	private:
		Vector2f newPos;
	};


	class KRAKOA_API WindowClosedEvent : public ApplicationEvent
	{
	public:
		WindowClosedEvent() = default;

		USE_RESULT std::string ToString() const noexcept override
		{
			return "Window Closed Event!";
		}

		EVENT_CLASS_TYPE(WINDOW_CLOSE)
	};


	class KRAKOA_API TickEvent : public ApplicationEvent
	{
	public:
		TickEvent() = default;

		USE_RESULT std::string ToString() const noexcept override
		{
			return "Tick Event!";
		}

		EVENT_CLASS_TYPE(APP_TICK)
	};

	class UpdateEvent : ApplicationEvent
	{
	public:
		UpdateEvent() = default;

		USE_RESULT std::string ToString() const noexcept override
		{
			return "Update Event!";
		}

		EVENT_CLASS_TYPE(APP_UPDATE)
	};

	class RenderEvent : ApplicationEvent
	{
	public:
		RenderEvent() = default;

		USE_RESULT std::string ToString() const noexcept override
		{
			return "Render Event!";
		}

		EVENT_CLASS_TYPE(APP_RENDER)
	};
}


