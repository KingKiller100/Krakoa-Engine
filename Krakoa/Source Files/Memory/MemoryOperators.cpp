#include "Precompile.hpp"
#include "MemoryOperators.hpp"

#include "MemoryPool.hpp"
#include "Memory Structures/HeapBase.hpp"
#include "Memory Structures/MemoryTypes.hpp"
#include "Memory Structures/HeapFactory.hpp"

#include "../Core/Logging/MemoryLogger.hpp"

#include <cassert>

using namespace memory;

static bool isMemPoolInitialized = false;

static size_t allocIter = 0;

void* operator new(const size_t bytes, HeapBase* pHeap) // Pads Control Blocks
{
#ifndef  KRAKOA_RELEASE
	MEM_ASSERT((bytes != 0) || bytes < CAST(size_t, -1));

	auto& memPool = MemoryPool::Reference();
	
	if (!isMemPoolInitialized)
	{
		memPool.Initialize(200, kmaths::BytesUnits::MEGA);
		isMemPoolInitialized = true;
	}

	const size_t requestedBytes = AllocHeaderSize + bytes + SignatureSize; // Alignment in memory
	auto* pBlock = memPool.Allocate(requestedBytes);
	auto* pHeader = REINTERPRET(AllocHeader*, pBlock);

	pHeader->signature = KRK_MEMSYSTEM_START_SIG;
	pHeader->id = allocIter++;
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
	auto* pMemEnd = REINTERPRET(AllocHeader::Signature_Ptr_Type, pMemStart + bytes);
	*pMemEnd = KRK_MEMSYSTEM_END_SIG;

	pHeap->Allocate(requestedBytes);

	return pMemStart; // Returns pointer to the start of the object's data
#else
	return malloc(bytes);
#endif	
}

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

void operator delete(void* ptr)
{
	if (!ptr)
		return;

#ifndef KRAKOA_RELEASE
	auto* pHeader = AllocHeader::GetHeaderFromPointer(ptr);

	auto& pHeap = pHeader->pHeap;
	const auto bytes = pHeader->bytes;
	auto& pPrev = pHeader->pPrev;
	auto& pNext = pHeader->pNext;

	const auto totalBytes = AllocHeaderSize + bytes + SignatureSize;

	if (!pPrev && !pNext) // Both null
		pHeap->SetPrevAddress(nullptr);
	else // Only one null
	{
		if (pNext)
			pNext->pPrev = pHeader->pPrev;
		else
			pHeap->SetPrevAddress(pPrev);

		if (pPrev)
			pPrev->pNext = pHeader->pNext;
	}

	pPrev = pNext = nullptr;
	
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

