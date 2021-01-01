#include "Precompile.hpp"
#include "MemoryOperators.hpp"

#include "MemoryPool.hpp"
#include "MemoryStructures/Heap.hpp"
#include "MemoryStructures/MemoryTypeSizes.hpp"
#include "MemoryStructures/HeapFactory.hpp"

#include "../Debug/Debug.hpp"

void* operator new [](const size_t bytes, memory::Heap* pHeap)
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

void* operator new(const size_t bytes, memory::Heap* pHeap) // Pads Control Blocks
{
#ifndef  KRAKOA_RELEASE
	MEM_ASSERT((bytes != 0) || bytes < CAST(size_t, -1),
		"Illegal amount of bytes requested");

	const size_t requestedBytes = memory::AllocHeaderSize + bytes + memory::SignatureSize; // Alignment in memory
	auto* pBlock = CAST(kmaths::Byte_Type*, malloc(requestedBytes)); // memory::MemoryPool::Reference().Allocate(requestedBytes);
	auto* pHeader = REINTERPRET(memory::AllocHeader*, pBlock);
	return memory::AllocHeader::Create(pHeader, bytes, pHeap); // Returns pointer to the start of the object's data
#else
	return malloc(bytes);
#endif	
}

void operator delete [](void* ptr)
{
	operator delete(ptr);
}

void operator delete(void* ptr)
{
	if (!ptr) return;
	
#ifndef KRAKOA_RELEASE
	auto* pHeader = memory::AllocHeader::Destroy(ptr);
	free(pHeader);
#else
	free(ptr);
#endif
}

