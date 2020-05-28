#pragma once

#include "SimpleSingleton.hpp"
#include "MemoryOverrider.hpp"

namespace patterns
{
	template<typename T>
	class ManagerBase : public SimpleSingleton<T>, public MemoryOverrider<T>
	{
	public:
		~ManagerBase() noexcept override
			= default;
	};
}
