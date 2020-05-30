#pragma once
#include "../Memory/Memory Structures/TemplateHeap.hpp"
#include "../Memory/Memory Structures/HeapFactory.hpp"
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
				pHeap = memory::HeapFactory::CreateHeap<T>();
			
			return ::operator new(bytes, pHeap);
		}

		void* operator new[](const size_t bytes)
		{
			return MemoryOverrider::operator new(bytes);
		}

		void operator delete(void* pMemBlock)
		{
			::operator delete(pMemBlock);
		}

		void operator delete(void* pMemBlock, const size_t bytes)
		{
			::operator delete(pMemBlock);
		}

		void operator delete[](void* pMemBlock)
		{
			::operator delete[](pMemBlock);
		}

		void operator delete[](void* pMemBlock, const size_t bytes)
		{
			::operator delete[](pMemBlock);
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
		inline static memory::HeapBase* pHeap = nullptr;
	};
}
