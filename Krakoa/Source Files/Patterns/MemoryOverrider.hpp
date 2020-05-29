#pragma once
#include "../Memory/Heap.hpp"
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
			if (!pHeap)
				pHeap = memory::HeapFactory::CreateHeap(typeid(T).name());
			
			return ::operator new(bytes, pHeap);
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

		USE_RESULT static std::string GetHeapStatus() noexcept
		{
			return pHeap->GetStatus();
		}

		USE_RESULT static decltype(auto) WalkTheHeap()
		{
			return pHeap->WalkTheHeap();
		}

	private:
		inline static memory::Heap* pHeap = nullptr;
	};
}
