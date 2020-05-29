#pragma once

#include <HelperMacros.hpp>

#include <string>

namespace memory
{
class MemoryPool
{
public:
	void Allocate(const size_t bytes);
	void Deallocate(const size_t bytes);
	USE_RESULT size_t GetTotalBytes() const;
	USE_RESULT size_t GetMaxBytes() const;
	
	USE_RESULT size_t WalkTheHeap() const;
	USE_RESULT std::string GetReport() const;
	
	
private:
	size_t maxBytes;
};
}

