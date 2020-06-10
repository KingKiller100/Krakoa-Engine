#include "Precompile.hpp"
#include "MemoryOperators.hpp"

#include "MemoryPool.hpp"
#include "Memory Structures/HeapBase.hpp"
#include "Memory Structures/MemoryTypes.hpp"
#include "Memory Structures/HeapFactory.hpp"

#include "../Core/Logging/MemoryLogger.hpp"

static size_t bookmarkIter = 0;
static bool isMemPoolInitialized = false;

void InitializeMemoryPool() noexcept;


void* operator new [](const size_t bytes, memory::HeapBase* pHeap)
{
	return operator new(bytes, pHeap);
}

void* operator new(const size_t bytes)
{
	return operator new(bytes, memory::HeapFactory::GetDefaultHeap());
}

void* operator new [](const size_t bytes)
{
	return operator new(bytes, memory::HeapFactory::GetDefaultHeap());
}

using namespace memory;
void* operator new(const size_t bytes, HeapBase* pHeap) // Pads Control Blocks
{
#ifndef  KRAKOA_RELEASE
	MEM_ASSERT((bytes != 0) || bytes < CAST(size_t, -1));

	if (!isMemPoolInitialized) InitializeMemoryPool();

	const size_t requestedBytes = AllocHeaderSize + bytes + SignatureSize; // Alignment in memory
	auto* pBlock = MemoryPool::Reference().Allocate(requestedBytes);
	auto* pHeader = REINTERPRET(AllocHeader*, pBlock);

	pHeader->signature = KRK_MEMSYSTEM_START_SIG;
	pHeader->bookmark = bookmarkIter++;
	pHeader->pHeap = pHeap;
	pHeader->bytes = bytes;
	pHeader->pPrev = pHeader->pNext = nullptr;

	if (pHeader->pHeap->GetPrevAddress())
	{
		auto* pPrevHeader = pHeader->pHeap->GetPrevAddress();
		pHeader->pPrev = pPrevHeader;
		pPrevHeader->pNext = pHeader;
	}

	pHeader->pHeap->SetPrevAddress(pHeader);

	auto* pMemStart = pBlock + AllocHeaderSize;
	auto* pMemEnd = REINTERPRET(AllocHeader::Signature_Type*, pMemStart + bytes);
	*pMemEnd = KRK_MEMSYSTEM_END_SIG;

	pHeap->Allocate(requestedBytes);

	return pMemStart; // Returns pointer to the start of the object's data
#else
	return malloc(bytes);
#endif	
}

void InitializeMemoryPool() noexcept
{
#ifdef KRAKOA_TEST
	MemoryPool::Reference().Initialize(5, kmaths::BytesUnits::MEBI);
#else
	MemoryPool::Reference().Initialize(200, kmaths::BytesUnits::MEGA);
#endif
	isMemPoolInitialized = true;
}

void operator delete(void* ptr)
{
	if (!ptr)
		return;

#ifndef KRAKOA_RELEASE
	auto* pHeader = AllocHeader::GetHeaderFromPointer(ptr);

	auto* pHeap = pHeader->pHeap;
	const auto bytes = pHeader->bytes;
	auto* pPrev = pHeader->pPrev;
	auto* pNext = pHeader->pNext;

	const auto totalBytes = AllocHeaderSize + bytes + SignatureSize;

	if (!pPrev && !pNext) // Both null
		pHeap->SetPrevAddress(nullptr);
	else // Maybe one null
	{
		if (pNext)
			pNext->pPrev = pHeader->pPrev;

		if (pPrev)
			pPrev->pNext = pHeader->pNext;
		else
			pHeap->SetPrevAddress(pNext);

		pPrev = pNext = nullptr;
	}

	pHeap->Deallocate(totalBytes);
	MemoryPool::Reference().Deallocate(pHeader, totalBytes);
#else
	free(ptr);
#endif
}

void operator delete [](void* ptr)
{
	operator delete(ptr);
}

