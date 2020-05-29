#include "Precompile.hpp"
#include "MemoryOperators.hpp"

#include "Memory Structures/HeapBase.hpp"
#include "Memory Structures/MemoryTypes.hpp"
#include "Memory Structures/HeapFactory.hpp"

#include <cassert>

#define MEM_ASSERT(condition) assert(condition)

void* operator new(const size_t bytes, memory::HeapBase* pHeap) // Pads AllocHeader
{
	const size_t requestedBytes = memory::AllocHeaderBytes + bytes + memory::SignatureBytes; // Alignment in memory
	auto* pBlock = CAST(memory::Byte_Ptr_Type, malloc(requestedBytes));
	auto* pHeader = REINTERPRET(memory::AllocHeader*, pBlock);

	pHeader->signature = KRK_MEMSYSTEM_SIGNATURE;
	pHeader->pHeap = pHeap;
	pHeader->bytes = bytes;
	pHeader->pPrev = pHeader->pNext = nullptr;

	if (pHeader->pHeap->GetPrevAddress())
	{
		auto* pPrevHeader = CAST(memory::AllocHeader*, pHeader->pHeap->GetPrevAddress());
		pHeader->pPrev = pPrevHeader;
		pPrevHeader->pNext = pHeader;
	}

	pHeader->pHeap->SetPrevAddress(pHeader);

	auto* pMemStart = pBlock + memory::AllocHeaderBytes;
	auto* pMemEnd = REINTERPRET(memory::AllocHeader::Signature_Ptr_Type, pMemStart + bytes);
	*pMemEnd = KRK_MEMSYSTEM_ENDMARKER;

	pHeap->Allocate(requestedBytes);

	return pMemStart; // Returns the start of the object's to read
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
	auto* pData = CAST(memory::Byte_Ptr_Type, ptr);

	auto* pHeader = REINTERPRET(memory::AllocHeader*, pData - memory::AllocHeaderBytes);

	MEM_ASSERT(pHeader->signature == KRK_MEMSYSTEM_SIGNATURE);

	auto& pHeap = pHeader->pHeap;
	const auto bytes = pHeader->bytes;
	auto& pPrev = pHeader->pPrev;
	auto& pNext = pHeader->pNext;

	const auto totalBytes = memory::AllocHeaderBytes + bytes + memory::SignatureBytes;

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

	auto* pMemEnd = REINTERPRET(memory::AllocHeader::Signature_Ptr_Type, pData + bytes);

	MEM_ASSERT(*pMemEnd == KRK_MEMSYSTEM_ENDMARKER);

	pHeap->Deallocate(totalBytes);

	pHeap->CallObjectDestructor(ptr);
	
	ptr = nullptr;

	free(pHeader);
}

void operator delete [](void* ptr)
{
	operator delete(ptr);
}

