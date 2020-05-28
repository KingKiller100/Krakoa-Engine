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
			if (!heap)
				heap = memory::HeapFactory::CreateHeap(typeid(T).name());
			
			return ::operator new(bytes, heap);
		}

		void* operator new[](const size_t bytes)
		{
			return MemoryOverrider::operator new(bytes);
		}

		void operator delete(void* ptr)
		{
			::operator delete(ptr);
		}

		void operator delete(void* ptr, const size_t bytes)
		{
			::operator delete(ptr);
		}

		void operator delete[](void* ptr)
		{
			::operator delete[](ptr);
		}

		void operator delete[](void* ptr, const size_t bytes)
		{
			::operator delete[](ptr);
		}

	protected:
		inline static memory::Heap* heap = nullptr;
	};
}
