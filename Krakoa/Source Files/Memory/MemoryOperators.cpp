#include "Precompile.hpp"
#include "MemoryOperators.hpp"

#include "MemoryPool.hpp"
#include "Memory Structures/HeapBase.hpp"
#include "Memory Structures/MemoryTypes.hpp"
#include "Memory Structures/HeapFactory.hpp"

#include "../Core/Logging/MemoryLogger.hpp"

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


void* operator new(const size_t bytes, memory::HeapBase* pHeap) // Pads Control Blocks
{
#ifndef  KRAKOA_RELEASE
	MEM_ASSERT((bytes != 0) || bytes < CAST(size_t, -1));

	if (!isMemPoolInitialized) InitializeMemoryPool();

	const size_t requestedBytes = memory::AllocHeaderSize + bytes + memory::SignatureSize; // Alignment in memory
	auto* pBlock = CAST(kmaths::Byte_Type*, malloc(requestedBytes)); // memory::MemoryPool::Reference().Allocate(requestedBytes);
	auto* pHeader = REINTERPRET(memory::AllocHeader*, pBlock);
	return memory::AllocHeader::Initialize(pHeader, bytes, pHeap); // Returns pointer to the start of the object's data
#else
	return malloc(bytes);
#endif	
}

void InitializeMemoryPool() noexcept
{
#ifdef KRAKOA_TEST
	memory::MemoryPool::Reference().Initialize(5, kmaths::BytesUnits::MEBI);
#else
	memory::MemoryPool::Reference().Initialize(200, kmaths::BytesUnits::MEGA);
#endif
	isMemPoolInitialized = true;
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

