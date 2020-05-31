#include "Precompile.hpp"
#include "MemoryPool.hpp"

#include "Memory Structures/MemoryTypes.hpp"

#include "../Core/Logging/MemoryLogger.hpp"


namespace memory
{
	bool MemoryPool::active = false;

	constexpr auto noAvailableSpaceFlag = -1;

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

	Byte_Ptr_Type MemoryPool::Allocate(const size_t requestedBytes)
	{
		auto& pool = GetSubPoolIndex(requestedBytes);

		auto& nextFree = pool.pNextFree;
		auto* pBlock = pool.pNextFree;
		nextFree += requestedBytes;

		return pBlock;
	}

	SubPool& MemoryPool::GetSubPoolIndex(const size_t requestedBytes)
	{
		int index = 0;

		for (; index < SubPoolSize; ++index)
		{
			const auto& currentPool = subPools[index];
			const auto& head = currentPool.pHead;
			const auto& nextFree = currentPool.pNextFree;

			auto* const headByteVal = CAST(Byte_Ptr_Type, head);
			const auto currentSpace = static_cast<size_t>(nextFree - headByteVal);
			const auto newSpace = currentSpace + requestedBytes;

			if (currentPool.capacity >= newSpace) // TRUE - we have space to allocate FALSE - No more space within this pool
				break;
		}

		MEM_ASSERT(index != noAvailableSpaceFlag);

		return subPools[index];
	}

	void MemoryPool::CreateNewPool(const size_t capacity, const size_t index)
	{
		static std::array<bool, SubPoolSize> usedIndex{};

		if (usedIndex[index])
			throw std::bad_alloc(); // Attempting to create a pool that already exists

		auto& pool = subPools[index];

		pool.pHead = malloc(capacity);
		MEM_ASSERT(pool.pHead > nullptr);
		pool.capacity = capacity;
		pool.pNextFree = CAST(Byte_Ptr_Type, pool.pHead);

		memset(pool.pHead, 0, capacity);

		usedIndex[index] = true;
	}

	void MemoryPool::Deallocate(AllocHeader* pHeader, const size_t bytesToDelete)
	{
		auto& pool = FindPointerOwner(pHeader);
		memset(pHeader, 0, bytesToDelete);
		pool.pNextFree = REINTERPRET(Byte_Ptr_Type, pHeader);
		DefragHeap(pool, bytesToDelete);
	}

	SubPool& MemoryPool::FindPointerOwner(void* pHeader)
	{
		const auto* const addressVal = CAST(Byte_Ptr_Type, pHeader);

		for (auto i = 0; i < SubPoolSize; ++i)
		{
			const auto& pool = subPools[i];
			const auto* startPoint = CAST(Byte_Ptr_Type, pool.pHead);
			const auto* endPoint = startPoint + pool.capacity;

			if (startPoint >= addressVal
				&& endPoint <= addressVal)
				return subPools[i];
		}

		throw std::exception("Pointer not from any of our memory pools");
	}

	void MemoryPool::DefragHeap(SubPool& pool, const size_t deletedBytes)
	{
		auto* deadSpace = pool.pNextFree;
		auto* nextBlock = deadSpace + deletedBytes;

		do {
			auto* block = REINTERPRET(AllocHeader*, nextBlock);
			AllocHeader::Verify(block);
			
			memmove(deadSpace, block, block->bytes + MemoryControlBlockBytes);
			deadSpace += ;

		} while (AllocHeader::Verify(REINTERPRET(AllocHeader*, nextBlock)));
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
		return "";
	}

	MemoryPool& MemoryPool::Reference()
	{
		static Token token;
		static MemoryPool instance(token);

		return instance;
	}
}
