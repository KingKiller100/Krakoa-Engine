#pragma once

namespace memory
{
	class HeapBase;
}

void* operator new(const size_t bytes);
void* operator new[](const size_t bytes);

void* operator new(const size_t bytes, memory::HeapBase* pHeap);
void* operator new[](const size_t bytes, memory::HeapBase* pHeap);

void operator delete(void* ptr);
void operator delete[](void* ptr);
