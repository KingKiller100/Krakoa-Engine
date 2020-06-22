#include "Precompile.hpp"
#include "MemoryPool.hpp"

#include "MemoryErrors.hpp"
#include "Memory Structures/MemoryTypeSizes.hpp"

#include "../Core/Logging/MemoryLogger.hpp"

#include <Maths/kAlgorithms.hpp>

namespace memory
{
	using MemoryPoolLinkedList = MemoryLinkedList<void>;

	constexpr size_t DeadBlockSize = 4 * static_cast<size_t>(kmaths::BytesUnits::KIBI);
	constexpr size_t MPLLSize = MemoryLinkedListSize<void>;

	void* exampleDeadBlock = nullptr;

	using namespace kmaths;

	MemoryPool::MemoryPool(const size_t initialVolume, const size_t typeSize)
	{
		if (!exampleDeadBlock)
		{
			exampleDeadBlock = malloc(DeadBlockSize);
			memset(exampleDeadBlock, 0, DeadBlockSize);

			if (!exampleDeadBlock)
				throw debug::MemoryPoolError("Cannot allocate space for example dead memory block");
		}

		const auto capacity = initialVolume * typeSize;;
		CreateNewPool(capacity, 0);
	}

	MemoryPool::~MemoryPool() noexcept
	{
		ShutDown();
	}

	void MemoryPool::ShutDown()
	{
		for (auto& subPool : subPoolList)
		{
			if (!subPool.pHead)
				continue;

			free(subPool.pHead);
			subPool.pHead = subPool.pNextFree = nullptr;
		}
	}

	kmaths::Byte_Type* MemoryPool::Allocate(const size_t requestedBytes)
	{
		return GetBlockStartPtr(requestedBytes);
	}

	kmaths::Byte_Type* MemoryPool::GetBlockStartPtr(const size_t requestedBytes)
	{
		for (int index = 0; index < SubPoolSize; ++index)
		{
			auto& currentPool = subPoolList[index];

			if (!currentPool.pHead)
			{
				const auto nextCapacity = subPoolList[index - 1].capacity >> 1; // Next pool has half the capacity as the previous
				CreateNewPool(nextCapacity, index);
			}

			auto* pBlockStart = FindFreeBlock(currentPool, requestedBytes);

			if (!pBlockStart)
				continue;

			currentPool.remainingSpace -= requestedBytes;
			return pBlockStart;
		}

		throw debug::MemoryFullError();
	}

	void MemoryPool::CreateNewPool(const size_t capacity, const size_t index)
	{
		static std::array<bool, SubPoolSize> usedIndex{};
		if (usedIndex[index]) // Attempting to create a pool that already exists
			throw debug::MemoryPoolError("Attempting to initialize a pool already in uses");

		auto& pool = subPoolList[index];
		pool = SubPool(capacity);
		pool.pHead = malloc(capacity);
		pool.pNextFree = CAST(kmaths::Byte_Type*, pool.pHead);
		memset(pool.pHead, 0, capacity);

		MEM_ASSERT(pool.pHead);
		usedIndex[index] = true;
	}

	kmaths::Byte_Type* MemoryPool::FindFreeBlock(SubPool& pool, const size_t requestedBytes) const
	{
		if (CheckBlockIsDead(pool.pNextFree, requestedBytes))
		{
			auto* pBlockStart = reinterpret_cast<MemoryPoolLinkedList*>(pool.pNextFree);
			MemoryPoolLinkedList::CreateLinkedList(pBlockStart)
			pool.pNextFree += requestedBytes + MPLLSize;
			MoveNextFreePointer(pool.pNextFree);
			return pBlockStart + MPLLSize;
		}

		if (IsNegative(pool.pNextFree - static_cast<Byte_Type*>(pool.pHead)))
			throw debug::MemoryPoolError("Distance from pool's head to the"
				" pool's next free space pointers is negative!");

		auto* pNextFree = pool.pNextFree;
		auto* const prevFree = pNextFree;

		do {
			auto* pLList = reinterpret_cast<MemoryPoolLinkedList*>(pNextFree);

			if (MemoryPoolLinkedList::VerifyLinkedList(pLList))
				pNextFree += pLList->bytes + MPLLSize;
			else
			{
				auto maxLoops = requestedBytes;
				auto* const pEndAddress = static_cast<Byte_Type*>(pool.pHead) + pool.capacity;

				while (maxLoops-- > 0 && (pNextFree + requestedBytes) <= pEndAddress)
				{
					pLList = reinterpret_cast<MemoryPoolLinkedList*>(pNextFree);

					if (!MemoryPoolLinkedList::VerifyLinkedList(pLList))
						pNextFree++;
					else
					{
						pNextFree += MPLLSize + pLList->bytes;
						break;
					}
				}

				if ((pNextFree + requestedBytes) <= pEndAddress)
				{
					pool.pNextFree = prevFree;
					return nullptr;
				}
			}
		} while (!CheckBlockIsDead(pNextFree, requestedBytes));

		MoveNextFreePointer(pool.pNextFree);

		return pNextFree;
	}

	bool MemoryPool::CheckBlockIsDead(const Byte_Type* pNextFree, const size_t requestedBytes) const
	{
		if (requestedBytes <= DeadBlockSize)
		{
			return (memcmp(
				pNextFree,
				exampleDeadBlock,
				requestedBytes)
				== 0);
		}

		auto initialLoops = requestedBytes / DeadBlockSize;
		const auto remainingSize = requestedBytes % DeadBlockSize;

		while (initialLoops-- > 0)
		{
			if (memcmp(
				pNextFree + (DeadBlockSize * initialLoops),
				exampleDeadBlock,
				DeadBlockSize)
				!= 0)
				return false;
		}
		
		return (memcmp(pNextFree, exampleDeadBlock, remainingSize) != 0);
	}

	void MemoryPool::MoveNextFreePointer(kmaths::Byte_Type*& pNextFree)
	{
		auto* pLinkedList = reinterpret_cast<MemoryPoolLinkedList*>(pNextFree);
		while (MemoryPoolLinkedList::VerifyLinkedList(pLinkedList))
		{
			pNextFree += pLinkedList->bytes + MPLLSize;
			pLinkedList = reinterpret_cast<MemoryPoolLinkedList*>(pNextFree);
		}
	}
	

	void MemoryPool::Deallocate(void* pBlockStart, const size_t objectBytesToDelete)
	{
		auto* pFreeAddress = REINTERPRET(Byte_Type*, pBlockStart) - MPLLSize;

		auto& pool = FindPointerOwner(pBlockStart);
		memset(pBlockStart, 0, objectBytesToDelete);

		if (pFreeAddress < pool.pNextFree)
			pool.pNextFree = pFreeAddress;

		pool.remainingSpace += objectBytesToDelete;
	}

	SubPool& MemoryPool::FindPointerOwner(void* pHeader)
	{
		const auto* const pAddress = CAST(kmaths::Byte_Type*, pHeader);

		for (auto i = 0; i < SubPoolSize; ++i)
		{
			const auto& pool = subPoolList[i];
			const auto* pStartAddress = CAST(kmaths::Byte_Type*, pool.pHead);
			const auto* pEndAddress = pStartAddress + pool.capacity;

			if (pStartAddress <= pAddress
				&& pEndAddress > pAddress)
				return subPoolList[i];
		}

		throw debug::UnknownPointerError();
	}

	size_t MemoryPool::GetBytes() const
	{
		size_t currentStorage = 0;
		for (auto& pool : subPoolList)
			currentStorage += (pool.capacity - pool.remainingSpace);
		return currentStorage;
	}

	size_t MemoryPool::GetMaxBytes() const
	{
		size_t maxBytes = 0;
		for (auto& pool : subPoolList)
			maxBytes += pool.capacity;

		return maxBytes;
	}

	std::string MemoryPool::GetStatus() const
	{
		return "";
	}
}
