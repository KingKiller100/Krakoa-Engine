#pragma once

#include <Events System/Event.hpp>

namespace krakoa::events
{
	class KRAKOA_API ApplicationEvent : public Event
	{
	public:
		ApplicationEvent();
		~ApplicationEvent();

	private:

	};
}


