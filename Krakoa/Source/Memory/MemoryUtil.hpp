#pragma once
#include <utility>

namespace memory
{
	template<typename T>
	T* Allocate(size_t bytes)
	{
		auto* block = static_cast<T*>(std::malloc(bytes));
		std::memset(block, 0, bytes);
		return block;
	}
	
	template<typename T>
	T* Allocate()
	{
		return Allocate<T>(sizeof(T));
	}

	template<typename T>
	void Deallocate(T* ptr)
	{
		std::free(ptr);
	}
}
