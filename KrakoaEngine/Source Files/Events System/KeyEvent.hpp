#pragma once

#include <Events System/Event.hpp>
#include <Utility/Format/kFormatToString.hpp>

namespace krakoa::events
{
	class KRAKOA_API KeyEvent : public Event
	{
	public:
		USE_RESULT int GetKeyCode() const { return keycode; }

	protected:
		KeyEvent(int keycode)
			: keycode(keycode)
		{}

		EVENT_CLASS_CATEGORY(E_C_KEYBOARD | E_C_INPUT)
	protected:
		
		int keycode;
	};

	class KRAKOA_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int repeatCount)
			: KeyEvent(keycode), repeatCount(repeatCount)
		{}

		USE_RESULT int GetRepeatCount() const { return repeatCount; }

		USE_RESULT std::string ToString() const noexcept override
		{
			return klib::kFormat::ToString("Key Pressed Event: %d (%d Repeats)", keycode, repeatCount);
		}

		EVENT_CLASS_TYPE(KEY_PRESSED)
		
	private:
		int repeatCount;
	};

	class KRAKOA_API KeyReleasedEvent : public KeyEvent
	{
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode)
		{}

		USE_RESULT std::string ToString() const noexcept override
		{
			return klib::kFormat::ToString("Key Released: %d", keycode);
		}
		
		EVENT_CLASS_TYPE(KEY_RELEASED)
	};


}
