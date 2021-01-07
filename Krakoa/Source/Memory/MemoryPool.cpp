#include "Precompile.hpp"
#include "MemoryPool.hpp"

#include "MemoryDebug.hpp"
#include "MemoryErrors.hpp"
#include "MemoryStructures/MemoryTypeSizes.hpp"

#include "../Logging/MemoryLogger.hpp"

#include <Maths/kAlgorithms.hpp>
#include <Utility/String/kToString.hpp>


namespace memory
{
	void* exampleDeadBlock = nullptr;
	constexpr size_t DeadBlockSize = 4 * static_cast<size_t>(kmaths::BytesUnits::MEBI);

	constexpr size_t HeaderSize = sizeof(HeaderList);

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
			if (!subPool.pStartAddress)
				continue;

			free(subPool.pStartAddress);
			subPool.pStartAddress = subPool.pNextFree = nullptr;
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

			if (!currentPool.pStartAddress)
			{
				const auto nextCapacity = subPoolList[index - 1].capacity >> 1; // Next pool has half the capacity as the previous
				CreateNewPool(nextCapacity, index);
			}

			auto* pBlockStart = FindFreeBlock(currentPool, requestedBytes);

			if (!pBlockStart)
				continue;

			currentPool.remainingSpace -= requestedBytes + HeaderSize;
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
		pool.pStartAddress = malloc(capacity);
		MEM_ASSERT(pool.pStartAddress != nullptr, "Start address is null");
		pool.pNextFree = CAST(kmaths::Byte_Type*, pool.pStartAddress);
		memset(pool.pStartAddress, 0, capacity);
		

		usedIndex[index] = true;
	}

	kmaths::Byte_Type* MemoryPool::FindFreeBlock(SubPool& pool, const size_t requestedBytes) const
	{
		if (CheckBlockIsDead(pool.pNextFree, requestedBytes))
		{
			AddNewLink(pool, pool.pNextFree, requestedBytes);
			auto* pBlockStart = pool.pNextFree;
			MoveNextFreePointer(pool.pNextFree);
			return pBlockStart + HeaderSize;
		}

		if (kmaths::IsNegative(pool.pNextFree - static_cast<kmaths::Byte_Type*>(pool.pStartAddress)))
			throw debug::MemoryPoolError("Distance from pool's head to the"
				" pool's next free space pointers is negative!");

		auto* pNextFree = pool.pNextFree;
		auto* const prevFree = pNextFree;

		do {
			auto* pLList = reinterpret_cast<HeaderList*>(pNextFree);

			if (HeaderList::VerifyLinkedList(pLList))
				pNextFree += pLList->bytes + HeaderSize;
			else
			{
				auto maxLoops = requestedBytes;
				auto* const pEndAddress = static_cast<kmaths::Byte_Type*>(pool.pStartAddress) + pool.capacity;

				while (maxLoops-- > 0)
				{
					if ((pNextFree + requestedBytes) >= pEndAddress)
					{
						pool.pNextFree = prevFree;
						return nullptr;
					}

					pLList = reinterpret_cast<HeaderList*>(pNextFree);

					if (!HeaderList::VerifyLinkedList(pLList))
						pNextFree++;
					else
					{
						pNextFree += HeaderSize + pLList->bytes;
						break;
					}
				}
			}
		} while (!CheckBlockIsDead(pNextFree, requestedBytes));

		AddNewLink(pool, pNextFree, requestedBytes);
		MoveNextFreePointer(pool.pNextFree);

		return pNextFree;
	}

	void MemoryPool::AddNewLink(SubPool& pool, void* pNextBlock, const size_t bytes) const
	{
		static HeaderList* dummy = nullptr;

		if (!pool.ppHead)
		{
			dummy = static_cast<HeaderList*>(pNextBlock);
			pool.ppHead = &dummy;
			HeaderList::CreateLinkedList(*pool.ppHead, bytes, nullptr, nullptr);
			return;
		}

		auto* pCurrent = *pool.ppHead;

		while (HeaderList::VerifyLinkedList(pCurrent)
			&& pCurrent < pNextBlock
			&& pCurrent->pNext)
		{
			pCurrent = static_cast<HeaderList*>(pCurrent->pNext);
		}

		auto* pLinkedList = REINTERPRET(HeaderList*, pNextBlock);

		if (pCurrent > pLinkedList)
		{
			HeaderList::CreateLinkedList(pLinkedList, bytes, pCurrent->pPrev, pCurrent);
			pCurrent->pPrev = pLinkedList;
		}
		else
		{
			HeaderList::CreateLinkedList(pLinkedList, bytes, pCurrent, pCurrent->pNext);
			pCurrent->pNext = pLinkedList;
		}

		if (*pool.ppHead > pLinkedList)
			dummy = pLinkedList;

	}

	bool MemoryPool::CheckBlockIsDead(const kmaths::Byte_Type* pNextFree, const size_t requestedBytes) const
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
		auto* pLinkedList = reinterpret_cast<HeaderList*>(pNextFree);
		while (HeaderList::VerifyLinkedList(pLinkedList))
		{
			pNextFree += pLinkedList->bytes + HeaderSize;
			pLinkedList = reinterpret_cast<HeaderList*>(pNextFree);
		}
	}


	void MemoryPool::Deallocate(void* ptr, const size_t objectBytesToDelete)
	{
		auto* pHeaderList =
			REINTERPRET(HeaderList*, (REINTERPRET(kmaths::Byte_Type*, ptr) - HeaderSize));
		
		auto& pool = FindOwner(pHeaderList);
		
		if (pHeaderList == *pool.ppHead)
		{
			pool.ppHead = &pHeaderList->pNext;
			pHeaderList->pNext->pPrev = nullptr;
		}

		memset(pHeaderList, 0, objectBytesToDelete + HeaderSize);

		auto* pBlockStart = REINTERPRET(kmaths::Byte_Type*, pHeaderList);
		
		if (pBlockStart < pool.pNextFree)
			pool.pNextFree = pBlockStart;

		pool.remainingSpace += + HeaderSize + HeaderSize;
	}

	SubPool& MemoryPool::FindOwner(void* pBlock)
	{
		const auto* const pAddress = CAST(kmaths::Byte_Type*, pBlock);

		for (auto i = 0; i < SubPoolSize; ++i)
		{
			const auto& pool = subPoolList[i];
			const auto* pStartAddress = CAST(kmaths::Byte_Type*, pool.pStartAddress);
			const auto* pEndAddress = pStartAddress + pool.capacity;

			if (kmaths::InRange(pAddress, pStartAddress, pEndAddress))
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

	bool MemoryPool::IsEmpty() const noexcept
	{
		for (const auto& pool : subPoolList)
		{
			if (pool.capacity - pool.remainingSpace != 0)
				return false;
		}
		
		return true;
	}

	size_t MemoryPool::GetAllocationCount() const noexcept
	{
		size_t count = 0;
		for (const auto& pool : subPoolList)
		{
			if (!pool.pStartAddress || !pool.ppHead)
				continue;
			
			auto* pCurrentHeader = *pool.ppHead;
			while (pCurrentHeader)
			{
				count++;
				pCurrentHeader = pCurrentHeader->pNext;
			}
		}
		
		return count;
	}

	std::string MemoryPool::GetStatus(const char* type) const
	{
		std::string status;

		status += klib::kString::ToString(R"(
Type: {0}
Bytes: {1}
Capacity: {2}
Allocations: {3}
)", 
type,
IsEmpty() ? 0 : GetBytes(),
GetMaxBytes(),
GetAllocationCount());
		
		return status;
	}
}
