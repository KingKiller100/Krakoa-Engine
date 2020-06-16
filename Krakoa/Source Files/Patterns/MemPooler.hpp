#pragma once

#include "../Memory/MemoryPool.hpp"

#include <HelperMacros.hpp>

namespace patterns
{
	template<typename T>
	class MemPooler
	{
	public:
		void* operator new (size_t bytes)
		{
			if (!memoryPool)
				memoryPool = new memory::MemoryPool(10000, sizeof(T));
			return memoryPool->Allocate(bytes);
		}

		void operator delete(void* ptr)
		{
			auto* pHeader = memory::AllocHeader::GetHeaderFromPointer(ptr);
			memoryPool->Deallocate(pHeader, pHeader->bytes);
		}

		static size_t GetCurrentBytes() noexcept
		{
			return memoryPool->GetBytes();
		}

		static size_t GetCurrentCapacity() noexcept
		{
			return memoryPool->GetMaxBytes();
		}
		
	private:
		inline static memory::MemoryPool* memoryPool = nullptr;
	};
}
