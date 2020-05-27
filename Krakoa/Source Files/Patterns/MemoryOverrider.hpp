#pragma once
#include "../Memory/HeapFactory.hpp"
#include "../Memory/MemoryOperators.hpp"

namespace patterns
{
	template<typename T>
	class MemoryOverrider
	{
	public:
		void* operator new(const size_t bytes)
		{
			return krakoa::operator new(bytes, heap);
		}

		void* operator new[](const size_t bytes)
		{
			return  MemoryOverrider::operator new(bytes);
		}

		void operator delete(void* ptr)
		{
			krakoa::operator delete(ptr);
		}

		void operator delete(void* ptr, const size_t bytes)
		{
			krakoa::operator delete(ptr);
		}

		void operator delete[](void* ptr)
		{
			krakoa::operator delete[](ptr);
		}

		void operator delete[](void* ptr, const size_t bytes)
		{
			krakoa::operator delete[](ptr);
		}

	private:
		static krakoa::Heap* heap = krakoa::HeapFactory::CreateHeap(typeid(T).name());
	};
}
