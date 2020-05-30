#include "Precompile.hpp"
#include "DefaultHeap.hpp"

namespace memory
{
	DefaultHeap::DefaultHeap() noexcept
		: HeapBase("Default")
	{}

	DefaultHeap::~DefaultHeap() noexcept
		= default;
}
