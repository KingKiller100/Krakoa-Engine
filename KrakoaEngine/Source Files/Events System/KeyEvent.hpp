#pragma once

#include <Events System/Event.hpp>

namespace krakoa::events
{
	class KRAKOA_API KeyEvent : public Event
	{

	};

	class KRAKOA_API KeyPressedEvent : public KeyEvent
	{

	};

	class KRAKOA_API KeyReleasedEvent : public KeyEvent
	{

	};


}
