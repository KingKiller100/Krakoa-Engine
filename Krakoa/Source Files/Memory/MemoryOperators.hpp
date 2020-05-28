#pragma once

#include "HeapFactory.hpp"

void* operator new(const size_t bytes);
void* operator new[](const size_t bytes);

void* operator new(const size_t bytes, memory::Heap* pHeap);
void* operator new[](const size_t bytes, memory::Heap* pHeap);

void operator delete(void* ptr);
void operator delete[](void* ptr);
