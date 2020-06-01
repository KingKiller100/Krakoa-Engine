#include "Precompile.hpp"
#include "MemoryPool.hpp"

#include "Memory Structures/MemoryTypes.hpp"

#include "../Core/Logging/MemoryLogger.hpp"


namespace memory
{
	using namespace kmaths;
	
	constexpr auto noAvailableSpaceFlag = -1;

	MemoryPool::MemoryPool(Token&) noexcept
		:poolIncrementBytes(500 * static_cast<size_t>(BytesUnits::KILO))
	{}

	MemoryPool::~MemoryPool() noexcept
	{
		ShutDown();
	}

	void MemoryPool::Initialize(const size_t volume, const kmaths::BytesUnits units)
	{
		const auto capacity = volume * static_cast<size_t>(units);
		CreateNewPool(capacity, 0);
	}

	void MemoryPool::ShutDown()
	{
		for (auto& subPool : subPoolList)
		{
			if (!subPool.pHead)
				continue;

			free(subPool.pHead);
			subPool.pNextFree = nullptr;
			subPool.capacity = 0;
		}
	}

	kmaths::Byte_Type* MemoryPool::Allocate(const size_t requestedBytes)
	{
		auto& pool = GetSubPoolIndex(requestedBytes);

		auto* pBlock = pool.pNextFree;
		pool.pNextFree += requestedBytes;

#ifdef KRAKOA_DEBUG
		pool.remainingSpace -= requestedBytes;
#endif

		return pBlock;
	}

	SubPool& MemoryPool::GetSubPoolIndex(const size_t requestedBytes)
	{
		int index = 0;

		for (; index < SubPoolSize; ++index)
		{
			const auto& currentPool = subPoolList[index];

			if (!currentPool.pHead)
				CreateNewPool(poolIncrementBytes, index);
			
			const auto& head = currentPool.pHead;
			const auto& nextFree = currentPool.pNextFree;

			auto* const headByteVal = CAST(kmaths::Byte_Type*, head);
			const auto currentStorage = static_cast<size_t>(nextFree - headByteVal);
			const auto newStorage = currentStorage + requestedBytes;

			if (currentPool.capacity >= newStorage) // TRUE - we have space to allocate FALSE - No more space within this pool
				break;
		}

		if (index >= SubPoolSize) index = -1;
		
		MEM_ASSERT(index != noAvailableSpaceFlag);

		return subPoolList[index];
	}

	void MemoryPool::CreateNewPool(const size_t capacity, const size_t index)
	{
		static std::array<bool, SubPoolSize> usedIndex{};

		if (usedIndex[index])
			throw std::bad_alloc(); // Attempting to create a pool that already exists

		auto& pool = subPoolList[index];

		pool.pHead = malloc(capacity);
		
		MEM_ASSERT(pool.pHead > nullptr);
		pool.capacity = capacity;		
		pool.pNextFree = CAST(kmaths::Byte_Type*, pool.pHead);
		memset(pool.pHead, 0, capacity);
		
#ifdef KRAKOA_DEBUG
		pool.remainingSpace = capacity;
#endif

		usedIndex[index] = true;
	}

	void MemoryPool::Deallocate(AllocHeader* pHeader, const size_t bytesToDelete)
	{
		auto& pool = FindPointerOwner(pHeader);
		memset(pHeader, 0, bytesToDelete);
		pool.pNextFree = REINTERPRET(kmaths::Byte_Type*, pHeader);
		DefragHeap(pool, bytesToDelete);

#ifdef KRAKOA_DEBUG
		pool.remainingSpace += bytesToDelete;
#endif
	}

	SubPool& MemoryPool::FindPointerOwner(void* pHeader)
	{
		const auto* const addressVal = CAST(kmaths::Byte_Type*, pHeader);

		for (auto i = 0; i < SubPoolSize; ++i)
		{
			const auto& pool = subPoolList[i];
			const auto* startPoint = CAST(kmaths::Byte_Type*, pool.pHead);
			const auto* endPoint = startPoint + pool.capacity;

			if (startPoint <= addressVal
				&& endPoint > addressVal)
				return subPoolList[i];
		}

		throw std::exception("Pointer not from any of our memory pools");
	}

	void MemoryPool::DefragHeap(SubPool& pool, const size_t deletedBytes)
	{
		auto* currentDeadSpace = pool.pNextFree;
		auto* nextBlock = currentDeadSpace + deletedBytes;

		while (AllocHeader::VerifyHeader(REINTERPRET(AllocHeader*, nextBlock), false))
		{
			auto* block = REINTERPRET(AllocHeader*, nextBlock);
			auto* data = nextBlock + AllocHeaderSize;
			auto** pDataPtr = &data;
			*pDataPtr = (currentDeadSpace + AllocHeaderSize);
			const auto jumpBytes = block->bytes + ControlBlockSize;
			memmove(currentDeadSpace, block, jumpBytes);
			memset(block, 0, jumpBytes);
			currentDeadSpace += jumpBytes;
			nextBlock = currentDeadSpace + deletedBytes;
		}

		pool.pNextFree = currentDeadSpace;
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
