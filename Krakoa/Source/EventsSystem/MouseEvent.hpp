#pragma once
#include "Event.hpp"
#include "../Maths/Maths.hpp"

#include <Utility/String/kToString.hpp>

namespace krakoa::events
{
	class KRAKOA_API MouseEvent : public Event
	{
	public:
		EVENT_CLASS_CATEGORY(E_C_MOUSE | E_C_INPUT)

	protected:
		MouseEvent() = default;
	};

	class KRAKOA_API MouseMovedEvent : public MouseEvent
	{
	public:
		MouseMovedEvent(const maths::Point& pos)
			: position(pos)
		{}

		USE_RESULT maths::Point GetPosition() const noexcept
		{
			return position;
		}

		USE_RESULT float GetX() const noexcept
		{
			return position.X();
		}

		USE_RESULT float GetY() const noexcept
		{
			return position.Y();
		}


		USE_RESULT std::string ToString() const noexcept override
		{
			return klib::kString::ToString("Mouse Moved Event: (%.f, %.f)", position.X(), position.Y());
		}

		EVENT_CLASS_TYPE(MOUSE_MOVE)

	private:
		maths::Point position;
	};

	class KRAKOA_API MouseScrolledEvent : public MouseEvent
	{
	public:
		MouseScrolledEvent(const maths::Point& offset)
			: offset(offset)
		{}

		USE_RESULT maths::Point GetOffset() const noexcept
		{
			return offset;
		}

		USE_RESULT float GetX() const noexcept
		{
			return offset.X();
		}

		USE_RESULT float GetY() const noexcept
		{
			return offset.Y();
		}

		USE_RESULT std::string ToString() const noexcept override
		{
			return klib::kString::ToString("Mouse Scrolled Event: (%.f, %.f)", offset.X(), offset.Y());
		}

		EVENT_CLASS_TYPE(MOUSE_SCROLL)

	private:
		maths::Point offset;
	};

	class KRAKOA_API MouseButtonEvent : public MouseEvent
	{
	public:
		USE_RESULT int GetButton() const noexcept
		{
			return button;
		}

		EVENT_CLASS_TYPE(MOUSE_BUTTON)

	protected:
		MouseButtonEvent(const int button)
			: button(button)
		{}

	protected:
		int button;
	};

	class KRAKOA_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(const int button)
			: MouseButtonEvent(button)
		{}

		USE_RESULT std::string ToString() const noexcept override
		{
			return klib::kString::ToString("Mouse Button Pressed Event: %d", button);
		}

		EVENT_CLASS_TYPE(MOUSE_CLICK)
	};

	class KRAKOA_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(const int button)
			: MouseButtonEvent(button)
		{}

		USE_RESULT std::string ToString() const noexcept override
		{
			return klib::kString::ToString("Mouse Button Released Event: %d", button);
		}

		EVENT_CLASS_TYPE(MOUSE_RELEASE)
	};
}

