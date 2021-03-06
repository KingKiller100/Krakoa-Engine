﻿#pragma once

#include "../Memory/MemoryPool.hpp"

#include <HelperMacros.hpp>

namespace patterns
{
	template<typename T, size_t InitialSize>
	class MemPooler
	{
	public:
		static constexpr auto MemoryPoolSize = InitialSize;
		
	public:
		void* operator new (size_t bytes)
		{
			if (!memoryPool)
				memoryPool = new memory::MemoryPool(InitialSize, sizeof(T));
			return memoryPool->Allocate(bytes);
		}

		void operator delete(void* ptr)
		{
			if (!memoryPool) return;
			
			memoryPool->Deallocate(ptr, sizeof(T));
		}

		virtual ~MemPooler()
			= default;

		static void TerminatePool() noexcept
		{
			delete memoryPool;
			memoryPool = nullptr;
		}
		
		USE_RESULT static size_t GetCurrentBytes() noexcept
		{
			return memoryPool->GetBytes();
		}

		USE_RESULT static size_t GetCurrentCapacity() noexcept
		{
			return memoryPool->GetMaxBytes();
		}

		USE_RESULT static std::string GetStatus() noexcept
		{
			return memoryPool->GetStatus(typeid(T).name());
		}

	private:
		inline static memory::MemoryPool* memoryPool = nullptr;
	};
}
