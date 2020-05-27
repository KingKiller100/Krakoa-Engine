#include "Precompile.hpp"
#include "MemoryOperators.hpp"

#include "Heap.hpp"
#include "MemoryTypes.hpp"

#include <Utility/Debug Helper/Exceptions/NotImplementedException.hpp>

#include <cassert>

void* operator new(const size_t bytes, krakoa::Heap* pHeap) // Pads AllocHeader
{
	const size_t requestedBytes = sizeof(krakoa::AllocHeader) + bytes + sizeof(unsigned); // Alignment in memory
	auto* pBlock = CAST(krakoa::Byte_Ptr, malloc(requestedBytes));
	auto* pHeader = REINTERPRET(krakoa::AllocHeader*, pBlock);

	pHeader->signature = KRK_MEMSYSTEM_SIGNATURE;
	pHeader->pHeap = pHeap;
	pHeader->bytes = CAST(unsigned, bytes);
	pHeader->pPrev = pHeader->pNext = nullptr;

	if (pHeader->pHeap->GetPrevAddress())
	{
		auto* pPrevAddress = CAST(krakoa::AllocHeader*, pHeader->pHeap->GetPrevAddress());
		pHeader->pPrev = pPrevAddress;
		pPrevAddress->pNext = pHeader;
	}

	pHeader->pHeap->SetPrevAddress(pHeader);

	auto* pMemStart = pBlock + sizeof(krakoa::AllocHeader);
	auto* pMemEnd = REINTERPRET(unsigned*, pMemStart + bytes);
	*pMemEnd = KRK_MEMSYSTEM_ENDMARKER;

	pHeap->Allocate(bytes);

	return pMemStart; // Returns the start of the object's to read
}

void* operator new [](const size_t bytes, krakoa::Heap* pHeap)
{
	return operator new(bytes, pHeap);
}

void* operator new(const size_t bytes)
{
	return operator new(bytes, krakoa::HeapFactory::GetDefaultHeap());
}

void* operator new [](const size_t bytes)
{
	return operator new(bytes, krakoa::HeapFactory::GetDefaultHeap());
}

void operator delete(void* ptr)
{
	auto* pData = CAST(krakoa::Byte_Ptr, ptr);

	auto* pHeader = REINTERPRET(krakoa::AllocHeader*, pData - sizeof(krakoa::AllocHeader));

	assert(pHeader->signature == KRK_MEMSYSTEM_SIGNATURE);

	auto& pHeap = pHeader->pHeap;
	const auto bytes = pHeader->bytes;
	auto& pPrev = pHeader->pPrev;
	auto& pNext = pHeader->pNext;

	const auto totalBytes = sizeof(krakoa::AllocHeader) + bytes + sizeof(unsigned);

	if (pNext)
		pNext->pPrev = pHeader->pPrev;
	else
		pHeap->SetPrevAddress(pPrev);

	if (pPrev)
		pPrev->pNext = pHeader->pNext;

	if (!pPrev && !pNext)
		pHeap->SetPrevAddress(nullptr);

	auto* pMemEnd = REINTERPRET(unsigned*, pData + bytes);

	assert(*pMemEnd == KRK_MEMSYSTEM_ENDMARKER);

	free(pHeader);

	pHeap->Deallocate(totalBytes);
}

void operator delete [](void* ptr)
{
	operator delete(ptr);
}

