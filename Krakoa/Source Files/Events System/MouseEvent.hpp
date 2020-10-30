#pragma once
#include "Event.hpp"

#include <Maths/Vectors/Vector2.hpp>
#include <Utility/String/kToString.hpp>

namespace krakoa::events
{
	EXPIMP_TEMPLATE template struct KRAKOA_API kmaths::Vector<float, 2>;

	using namespace kmaths;

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
		MouseMovedEvent(const Vector2f& pos)
			: position(pos)
		{}


		USE_RESULT Vector2f& GetPosition() noexcept
		{
			return position;
		}

		USE_RESULT const Vector2f& GetPosition() const noexcept
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
		Vector2f position;
	};

	class KRAKOA_API MouseScrolledEvent : public MouseEvent
	{
	public:
		MouseScrolledEvent(const Vector2f& offset)
			: offset(offset)
		{}

		USE_RESULT Vector2f& GetOffset() noexcept
		{
			return offset;
		}

		USE_RESULT const Vector2f& GetOffset() const noexcept
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
		Vector2f offset;
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
			return klib::kString::ToString("Mouse Clicked Event: %d", button);
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

