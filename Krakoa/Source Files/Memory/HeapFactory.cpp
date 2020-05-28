#include "Precompile.hpp"
#include "HeapFactory.hpp"

#include "Heap.hpp"

#include "../Core/Logging/CoreLogger.hpp"


namespace memory
{
	Heap* HeapFactory::defaultHeap = nullptr;
	HeapList HeapFactory::heaps;

	void HeapFactory::ShutDown() noexcept
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
		if (!defaultHeap)
		{
			defaultHeap = CAST(Heap*, malloc(sizeof(Heap)));
			defaultHeap->Initialize("Default");
		}

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

		if (index == 0)
		{
			defaultHeap->WalkHeap();
			return;
		}
		
		heaps[index]->WalkHeap();

	}

	HeapFactory::HeapFactory(Token)
	{}
}
