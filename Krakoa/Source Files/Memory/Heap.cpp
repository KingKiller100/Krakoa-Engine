#include "Precompile.hpp"
#include "Heap.hpp"

#include <Utility/Debug Helper/Exceptions/NotImplementedException.hpp>

namespace krakoa
{
	Heap::Heap(const char* name) noexcept
		: name(name),
		totalBytes(0),
		prevAddress(nullptr)
	{}

	Heap::~Heap() noexcept
		= default;

	Heap* Heap::Create(const char* name)
	{
		return new Heap(name);
	}

	void Heap::SetName(const char* n)
	{
		name = n;
	}

	const char* Heap::GetName() const
	{
		return name;
	}

	void Heap::Allocate(const size_t bytes)
	{
		totalBytes += bytes;
	}

	void Heap::Deallocate(const size_t bytes)
	{
		totalBytes -= bytes;
	}

	size_t Heap::GetTotalAllocatedBytes() const
	{
		return totalBytes;
	}

	void Heap::WalkHeap()
	{
		throw klib::kDebug::NotImplementedException(__FUNCSIG__ " Not Implemented");
	}

	void* Heap::GetPrevAddress() const
	{
		return prevAddress;
	}
}
