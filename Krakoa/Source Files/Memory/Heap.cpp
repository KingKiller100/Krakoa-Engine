#include "Precompile.hpp"
#include "Heap.hpp"

#include <Utility/Debug Helper/Exceptions/NotImplementedException.hpp>

namespace krakoa
{
	Heap::Heap(const char* name) noexcept
		: name(name),
		totalBytes(0),
		pPrevAddress(nullptr)
	{}

	Heap::~Heap() noexcept
		= default;

	void Heap::Initialize(const char* n) noexcept
	{
		name = n;
		totalBytes = 0;
		pPrevAddress = nullptr;
	}

	void Heap::SetName(const char* n) noexcept
	{
		name = n;
	}

	const char* Heap::GetName() const noexcept
	{
		return name;
	}

	void Heap::Allocate(const size_t bytes) noexcept
	{
		totalBytes += bytes;
	}

	void Heap::Deallocate(const size_t bytes) noexcept
	{
		totalBytes -= bytes;
	}

	size_t Heap::GetTotalAllocatedBytes() const noexcept
	{
		return totalBytes;
	}

	void Heap::WalkHeap() const
	{
		throw FUNC_NO_IMPL();
	}

	void* Heap::GetPrevAddress() const noexcept
	{
		return pPrevAddress;
	}

	void Heap::SetPrevAddress(void* prev) noexcept
	{
		pPrevAddress = prev;
	}

	Heap* Create(const char* name)
	{
		return new Heap(name);
	}
}
