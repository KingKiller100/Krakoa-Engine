#include "Precompile.hpp"
#include "MemoryPool.hpp"

#include "MemoryErrors.hpp"
#include "Memory Structures/MemoryTypes.hpp"

#include "../Core/Logging/MemoryLogger.hpp"

#include <Maths/kAlgorithms.hpp>

namespace memory
{
	using namespace kmaths;

	constexpr auto noAvailableSpaceFlag = -1;
	constexpr auto deadBlockSize = 100 * static_cast<size_t>(BytesUnits::KILO);

	MemoryPool::MemoryPool(Token&) noexcept
	{
		exampleDeadBlock = malloc(deadBlockSize);
		memset(exampleDeadBlock, 0, deadBlockSize);
	}

	MemoryPool::~MemoryPool() noexcept
	{
		ShutDown();
	}

	void MemoryPool::Initialize(const size_t volume, const BytesUnits units)
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
			subPool.pHead = subPool.pNextFree = nullptr;
		}
	}

	kmaths::Byte_Type* MemoryPool::Allocate(const size_t requestedBytes)
	{
		auto* pBlockStart = GetBlockStartPtr(requestedBytes);
		return pBlockStart;
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
			
			if (pBlockStart)
			{
				currentPool.remainingSpace -= requestedBytes;
				return pBlockStart;
			}
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
			auto* allocPoint = pool.pNextFree;
			pool.pNextFree += ControlBlockSize + requestedBytes + SignatureSize;

			auto* currentHeader = reinterpret_cast<AllocHeader*>(pool.pNextFree);
			while (AllocHeader::VerifyHeader(currentHeader, false))
			{
				pool.pNextFree += ControlBlockSize + currentHeader->bytes + SignatureSize;
			}

			return allocPoint;
		}

		auto* pNextFree = pool.pNextFree + 1;
		auto* const prevFree = pool.pNextFree;
		const auto distance = pool.pNextFree - static_cast<Byte_Type*>(pool.pHead);

		if (IsNegative(distance))
			throw debug::MemoryPoolError("Distance between pool's head and"
				" pool's next free space pointers is negative!");

		bool isBlockFree = false;
		do
		{
			auto* pHeader = reinterpret_cast<AllocHeader*>(pNextFree);

			if (AllocHeader::VerifyHeader(pHeader, false))
			{
				pNextFree += pHeader->bytes + ControlBlockSize;
			}
			else
			{
				isBlockFree = CheckBlockIsDead(pNextFree, requestedBytes);
			}

			if (!isBlockFree)
			{
				auto maxLoops = requestedBytes;
				auto* const endAddress = static_cast<Byte_Type*>(pool.pHead) + pool.capacity;

				while (maxLoops > 0 && pNextFree < endAddress)
				{
					auto* const interruptingData = reinterpret_cast<AllocHeader*>(pNextFree);
					if (AllocHeader::VerifyHeader(interruptingData, false))
					{
						pNextFree += interruptingData->bytes + ControlBlockSize;
						break;
					}

					pNextFree++;
					maxLoops--;
				}

				if (endAddress <= pNextFree)
				{
					pool.pNextFree = prevFree;
					return nullptr;
				}
			}

		} while (!isBlockFree);

		if (*pool.pNextFree != 0)
		{
			while (*pool.pNextFree != 0)
			{
				pool.pNextFree++;
			}
		}

		return pNextFree;
	}

	bool MemoryPool::CheckBlockIsDead(const Byte_Type* pNextFree, const size_t requestedBytes) const
	{
		if (requestedBytes <= deadBlockSize)
		{
			if (memcmp(pNextFree, exampleDeadBlock, requestedBytes) == 0)
			{
				return true;
			}
		}
		else
		{
			bool passedTest = true;
			auto initialLoops = requestedBytes / deadBlockSize;

			while (initialLoops-- > 0)
			{
				if (memcmp(pNextFree + (deadBlockSize * initialLoops), exampleDeadBlock, deadBlockSize) != 0)
				{
					passedTest = false;
					break;
				}

			}

			if (passedTest)
			{
				const auto remainingSize = requestedBytes % deadBlockSize;
				if (memcmp(pNextFree, exampleDeadBlock, remainingSize) != 0)
					passedTest = false;
			}

			if (passedTest)
				return true;
		}

		return false;
	}

	void MemoryPool::MoveNextFreePointer(const kmaths::Byte_Type*& pNextFree)
	{
		constexpr auto initialIncrement = 1 << 6;
		auto increment = initialIncrement;
		const auto* originalPtr = pNextFree;

		
		if (*pNextFree != 0)
		{
			while (*pNextFree != 0)
			{
				pNextFree++;
			}
		}
		
	}


	void MemoryPool::Deallocate(AllocHeader* pHeader, const size_t bytesToDelete)
	{
		auto& pool = FindPointerOwner(pHeader);
		memset(pHeader, 0, bytesToDelete);

		if (REINTERPRET(Byte_Type*, pHeader) < pool.pNextFree)
			pool.pNextFree = REINTERPRET(kmaths::Byte_Type*, pHeader);

		pool.remainingSpace += bytesToDelete;
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

	MemoryPool& MemoryPool::Reference() noexcept
	{
		static Token token;
		static MemoryPool instance(token);

		return instance;
	}
}
