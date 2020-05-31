#include "Precompile.hpp"
#include "MemoryPool.hpp"

#include "Memory Structures/MemoryTypes.hpp"

#include "../Core/Logging/MemoryLogger.hpp"


namespace memory
{
	constexpr auto noAvailableSpaceFlag  = -1;
	
	MemoryPool::MemoryPool(Token&) noexcept
		: currentIndex(0)
	{}

	MemoryPool::~MemoryPool() noexcept
	{
		ShutDown();
	}

	void MemoryPool::Initialize(const size_t volume, const kmaths::BytesUnits units)
	{
		if (active)
			return;
		
		const auto capacity = volume * static_cast<size_t>(units);
		CreateNewPool(capacity, 0);
		active = true;
	}

	void MemoryPool::ShutDown()
	{
		for (auto& subPool : subPools)
		{
			if (!subPool.pHead)
				continue;
			
			free(subPool.pHead);
			subPool.pNextFree = nullptr;
			subPool.capacity = 0;
		}
	}

	Byte_Ptr_Type MemoryPool::Allocate(const size_t bytes)
	{
		const size_t requestedBytes = bytes; // Alignment in memory
		const auto index = GetSubPoolIndex(requestedBytes);
		auto& nextFree = subPools[index].pNextFree;
		auto* pBlock = subPools[index].pNextFree;
		nextFree += requestedBytes;
		return pBlock;
	}

	void MemoryPool::Deallocate(AllocHeader* pHeader, const size_t bytes)
	{
		const auto ptrValue = (size_t)pHeader;
		memset(pHeader, 0, bytes);
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

	int MemoryPool::GetSubPoolIndex(const size_t requestedBytes) const
	{
		int index = 0;

		for (; index < PoolSize; ++index)
		{
			const auto& currentPool = subPools[index];
			const auto& head = currentPool.pHead;
			const auto& nextFree = currentPool.pNextFree;

			auto* const headByteVal = CAST(Byte_Ptr_Type, head);
			const auto currentSpace = static_cast<size_t>(nextFree - headByteVal);
			const auto newSpace= currentSpace + requestedBytes;

			if (currentPool.capacity >= newSpace) // TRUE - we have space to allocate FALSE - No more space within this pool
				break;
		}

		MEM_ASSERT(index != noAvailableSpaceFlag);

		return index;
	}

	void MemoryPool::CreateNewPool(const size_t capacity, const size_t index)
	{
		static std::array<bool, PoolSize> usedIndex{};

		if (usedIndex[index])
			throw std::bad_alloc(); // Attempting to create a pool that already exists

		auto& subPool = subPools[index];

		subPool.pHead = malloc(capacity);
		MEM_ASSERT(subPool.pHead > nullptr);
		subPool.capacity = capacity;
		subPool.pNextFree = CAST(Byte_Ptr_Type, subPool.pHead);

		usedIndex[index] = true;
	}


	MemoryPool& MemoryPool::Reference()
	{
		static Token token;
		static MemoryPool instance(token);

		return instance;
	}
}
