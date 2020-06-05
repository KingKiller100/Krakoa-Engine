#include "Precompile.hpp"
#include "MemoryPool.hpp"

#include "Memory Structures/MemoryTypes.hpp"

#include "../Core/Logging/MemoryLogger.hpp"

#include "MemoryErrors.hpp"

namespace memory
{
	using namespace kmaths;

	constexpr auto noAvailableSpaceFlag = -1;
	constexpr auto deadBlockSize = 100 * static_cast<size_t>(BytesUnits::KILO);

	MemoryPool::MemoryPool(Token&) noexcept
		: poolIncrementBytes(100 * static_cast<size_t>(BytesUnits::KILO))
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
			subPool.capacity = 0;
		}
	}

	kmaths::Byte_Type* MemoryPool::Allocate(const size_t requestedBytes)
	{
		auto& pool = GetSubPoolWithSpace(requestedBytes);

		auto* pBlock = pool.pNextFree;
		pool.pNextFree += requestedBytes;

#ifndef KRAKOA_RELEASE
		pool.remainingSpace -= requestedBytes;
#endif

		return pBlock;
	}

	SubPool& MemoryPool::GetSubPoolWithSpace(const size_t requestedBytes)
	{
		for (int index = 0; index < SubPoolSize; ++index)
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
				return subPoolList[index];
		}

		throw debug::MemoryFullError();
	}

	bool MemoryPool::DoesPoolHaveEnoughSpace(SubPool& pool, const size_t requestedBytes)
	{
		if (CheckBlockIsDead(pool.pNextFree, requestedBytes))
			return true;

		auto*& pNextFree = pool.pNextFree;

		auto* const prevFree = pool.pNextFree;

		const auto distance = pool.pNextFree - pool.pHead;

		if (distance > requestedBytes)
			pNextFree = static_cast<Byte_Type*>(pool.pHead);

		bool isBlockFree = false;
		do
		{
			auto* pHeader = reinterpret_cast<AllocHeader*>(pNextFree);
			const auto isHeader = AllocHeader::VerifyHeader(pHeader, false);
			if (isHeader)
			{
				pNextFree += pHeader->bytes + ControlBlockSize;
			}
			else
			{
				isBlockFree = CheckBlockIsDead(pNextFree, requestedBytes);
			}

			if (!isBlockFree)
			{
				auto* const endAddress = pool.capacity + (Byte_Type*)pool.pHead;
				auto maxLoops = requestedBytes;
				for (; pNextFree != endAddress || maxLoops > 0; pNextFree++, maxLoops--)
				{
					auto* const interruptingData = reinterpret_cast<AllocHeader*>(pNextFree);
					if (AllocHeader::VerifyHeader(reinterpret_cast<AllocHeader*>(pNextFree), false))
					{
						pNextFree += interruptingData->bytes + ControlBlockSize;
						break;
					}
				}
			}

		} while (!isBlockFree);

		pool.pNextFree = prevFree;
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

#ifndef KRAKOA_RELEASE
		pool.remainingSpace = capacity;
#endif

		usedIndex[index] = true;
	}

	void MemoryPool::Deallocate(AllocHeader* pHeader, const size_t bytesToDelete)
	{
		auto& pool = FindPointerOwner(pHeader);
		memset(pHeader, 0, bytesToDelete);
		pool.pNextFree = REINTERPRET(kmaths::Byte_Type*, pHeader);

#ifndef KRAKOA_RELEASE
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

		throw debug::UnknownPointerError();
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
