#pragma once

#include <HelperMacros.hpp>

#include <string>

namespace memory
{
class MemoryPool
{
	struct Token{};
	
public:
	MemoryPool(Token&) noexcept;
	~MemoryPool() noexcept;

	void Initialize();
	
	void Allocate(const size_t bytes);
	void Deallocate(void* pBlock, const size_t bytes);

	USE_RESULT size_t GetTotalBytes() const;
	USE_RESULT size_t GetMaxBytes() const;
	
	USE_RESULT size_t WalkTheHeap() const;
	USE_RESULT std::string GetStatus() const;
	
	static MemoryPool& GetInstance();
private:
	size_t maxStorage;
	//std::array<void*, std::giga::num> 
};
}

