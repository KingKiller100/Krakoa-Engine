#pragma once

#include "SimpleSingleton.hpp"
#include "MemoryTracker.hpp"

namespace patterns
{
	template<typename T>
	class ManagerBase : public SimpleSingleton<T>, public MemoryTracker<T>
	{
	public:
		~ManagerBase() noexcept override
			= default;
	};
}
