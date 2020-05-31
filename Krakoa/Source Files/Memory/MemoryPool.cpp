#include "Precompile.hpp"
#include "MemoryPool.hpp"

#include "Memory Structures/MemoryTypes.hpp"

#include "../Core/Logging/MemoryLogger.hpp"

namespace memory
{
	void MemoryPool::Initialize(const size_t volume, const kmaths::BytesUnits units)
	{
		capacity = volume * static_cast<size_t>(units);
		head = malloc(capacity);
	}

	Byte_Ptr_Type MemoryPool::Allocate(const size_t bytes)
	{
		const size_t requestedBytes = AllocHeaderBytes + bytes + SignatureBytes; // Alignment in memory
		auto* const pBlock = pNextFree;
		MEM_ASSERT(IsSpaceAvailable(requestedBytes));

		pNextFree += requestedBytes;
		return pBlock;
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

	bool MemoryPool::IsSpaceAvailable(const size_t requestedBytes) const
	{
		auto* const headByteVal = CAST(Byte_Ptr_Type, head);
		const auto currentStorage = static_cast<size_t>(pNextFree - headByteVal);
		const auto newStorage = currentStorage + requestedBytes;
		return (capacity >= newStorage); // TRUE - we have space to allocate FALSE - No more space within this pool
	}

	MemoryPool& MemoryPool::GetInstance()
	{
		static Token token;
		static MemoryPool instance(token);

		return instance;
	}
}
