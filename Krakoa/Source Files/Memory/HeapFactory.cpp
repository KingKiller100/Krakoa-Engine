#include "Precompile.hpp"
#include "HeapFactory.hpp"

#include "Heap.hpp"

#include "../Core/Logging/CoreLogger.hpp"


namespace krakoa
{
	Heap* HeapFactory::defaultHeap = nullptr;
	HeapList HeapFactory::heaps;

	HeapFactory::HeapFactory(Token&&)
	{
		defaultHeap = CreateHeap("Default");
	}

	HeapFactory::~HeapFactory() noexcept
	{
		if (defaultHeap)
			free(defaultHeap);

		for (auto& heap : heaps)
			free(heap);
	}

	Heap* HeapFactory::CreateHeap(const char* name)
	{
		auto* heap = CAST(Heap*, malloc(sizeof(Heap)));
		heap->Initialize(name);
		heaps.emplace_back(heap);
		return heap;
	}

	Heap* HeapFactory::GetDefaultHeap() noexcept
	{
		return defaultHeap;
	}

	const HeapList& HeapFactory::GetHeaps()
	{
		return heaps;
	}

	void HeapFactory::WalkTheHeap(const size_t index)
	{
		KRK_FATAL(index < heaps.size(), "Index greater than size of heap container");

		KRK_INFO("Walking the heap");
		
		heaps[index]->WalkHeap();

	}
}
