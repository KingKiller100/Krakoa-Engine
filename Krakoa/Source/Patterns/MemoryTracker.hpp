#pragma once

#include "../Memory/MemoryOperators.hpp"
#include "../Memory/MemoryStructures/HeapFactory.hpp"
#include "../Memory/MemoryStructures/TemplateHeap.hpp"

namespace patterns
{
	template<typename T, const char* ParentName = nullptr>
	class MemoryTracker
	{
	public:
		void* operator new(const size_t bytes)
		{
			if (!pHeap)
			{
				pHeap = memory::HeapFactory::CreateHeap<T>();

				if (ParentName)
					AddToFamily(ParentName);
			}

			return ::operator new(bytes, pHeap);
		}

		void* operator new[](const size_t bytes)
		{
			return MemoryTracker::operator new(bytes);
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

		static bool AddToFamily(const char* parent)
		{
			return memory::HeapFactory::AddToParent(parent, pHeap);
		}

		USE_RESULT static memory::Heap* GetHeap() noexcept
		{
			return pHeap;
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
