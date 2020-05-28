#pragma once

#include "SimpleSingleton.hpp"
#include "MemoryOverrider.hpp"

namespace patterns
{
	template<typename T>
	class MemorySingleton : public SimpleSingleton<T>, public MemoryOverrider<T>
	{
	public:
		MemorySingleton()
		{
			MemoryOverrider<T>::heap = memory::HeapFactory::CreateHeap(typeid(T).name());
		}

		~MemorySingleton() noexcept override
			= default;
	};
}
