#pragma once

#include "Memory Structures/MemoryTypes.hpp"

#include <HelperMacros.hpp>
#include <Maths/BytesTypes.hpp>

#include <string>


namespace memory
{
class MemoryPool
{
	struct Token{};
	
public:
	MemoryPool(Token&) noexcept;
	~MemoryPool() noexcept;

	void Initialize(const size_t volume, const kmaths::BytesUnits units);
	
	Byte_Ptr_Type Allocate(const size_t bytes);
	void Deallocate(void* pBlock, const size_t bytes);

	USE_RESULT size_t GetTotalBytes() const;
	USE_RESULT size_t GetMaxBytes() const;
	
	USE_RESULT size_t WalkTheHeap() const;
	USE_RESULT std::string GetStatus() const;
	
	static MemoryPool& GetInstance();

private:
	bool IsSpaceAvailable(const size_t requestedBytes) const;
	
private:
	void* head;
	size_t capacity;
	Byte_Ptr_Type pNextFree;
};
}

