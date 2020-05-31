#include "Precompile.hpp"
#include "MemoryPool.hpp"


namespace memory
{
	void MemoryPool::Allocate(const size_t bytes)
	{
	}

	void MemoryPool::Deallocate(void* pBlock, const size_t bytes)
	{
	}

	size_t MemoryPool::GetTotalBytes() const
	{
		return 0;
	}

	size_t MemoryPool::GetMaxBytes() const
	{
		return 0;
	}

	size_t MemoryPool::WalkTheHeap() const
	{
		return 0;
	}

	std::string MemoryPool::GetStatus() const
	{
		return 0;
	}

	MemoryPool& MemoryPool::GetInstance()
	{
		static Token token;
		static MemoryPool instance(token);

		return instance;
	}
}
