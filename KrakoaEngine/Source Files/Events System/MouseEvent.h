#pragma once
#include <Events System/Event.hpp>
#include <Maths/Vectors/Vector2.hpp>
#include "Utility/Format/kFormatToString.hpp"

namespace krakoa::events
{
	using namespace kMaths;

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


		USE_RESULT Vector2f& GetPositionOffset() noexcept
		{
			return position;
		}

		USE_RESULT Vector2f GetPositionOffset() const noexcept
		{
			return position;
		}
		
		USE_RESULT std::string ToString() const noexcept override
		{
			return klib::kFormat::ToString("Mouse Moved Event: (%d, %d)", position.X(), position.Y());
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

		USE_RESULT Vector2f& GetPositionOffset() noexcept
		{
			return offset;
		}

		USE_RESULT Vector2f GetPositionOffset() const noexcept
		{
			return offset;
		}
		
		USE_RESULT std::string ToString() const noexcept override
		{
			return klib::kFormat::ToString("Mouse Scrolled Event: (%d, %d)", offset.X(), offset.Y());
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
			return klib::kFormat::ToString("Mouse Clicked Event: %d", button);
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
			return klib::kFormat::ToString("Mouse Button Released Event: %d", button);
		}

		EVENT_CLASS_TYPE(MOUSE_RELEASE)
	};

	
}

