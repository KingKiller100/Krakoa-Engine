#include "Precompile.hpp"
#include "HeapFactory.hpp"

#include "Heap.hpp"

#include "../Core/Logging/CoreLogger.hpp"

#include <Utility/String/kStringManipulation.hpp>

namespace memory
{
	Heap* HeapFactory::defaultHeap = nullptr;
	HeapFactory::HeapList HeapFactory::heaps;

	void HeapFactory::ShutDown() noexcept
	{
		for (auto& heap : heaps)
		{
			KRK_INFO(heap->Status());
			free(heap);
		}

		KRK_INFO(defaultHeap->Status());
		
		heaps.clear();
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

	const HeapFactory::HeapList& HeapFactory::GetHeaps()
	{
		return heaps;
	}

	std::string HeapFactory::WalkTheHeap(const size_t index)
	{
		KRK_FATAL(index < heaps.size(), "Index greater than current size of the HeapFactory");

		const auto& heap = heaps[index];

		const auto name = klib::kString::ToWriter(heap->GetName());
		KRK_INFO("Walking Heap: " + name);

		return heaps[index]->Status();
	}

	std::string HeapFactory::WalkTheDefaultHeap()
	{
		KRK_INFO("Walking Heap: Default");

		return defaultHeap->Status();
	}

	size_t HeapFactory::GetSize()
	{
		return heaps.size();
	}

	HeapFactory::HeapFactory(Token)
	{}
}
