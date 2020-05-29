#include "Precompile.hpp"
#include "HeapFactory.hpp"

#include "Heap.hpp"

#include "../Core/Logging/MemoryLogger.hpp"

#include <Utility/String/kStringManipulation.hpp>

namespace memory
{
	Heap* HeapFactory::defaultHeap = nullptr;
	HeapFactory::HeapList HeapFactory::heaps;

	void HeapFactory::Initialize() noexcept
	{
		MEM_INIT_LOGS();

#ifdef KRAKOA_RELEASE
		MEM_TOGGLE_LOGGING(); // Disable memory logging
#endif
	}

	void HeapFactory::ShutDown() noexcept
	{
		for (auto& heap : heaps)
		{
			MEM_INFO(heap->GetStatus());
			free(heap);
		}

		MEM_INFO(defaultHeap->GetStatus());
		
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
		MEM_FATAL(index < heaps.size(), "Index greater than current size of the HeapFactory");

		const auto& heap = heaps[index];

		const auto name = klib::kString::ToWriter(heap->GetName());
		MEM_INFO("Walking Heap: " + name);

		return heaps[index]->GetStatus();
	}

	std::string HeapFactory::WalkTheDefaultHeap()
	{
		MEM_INFO("Walking Heap: Default");

		return defaultHeap->GetStatus();
	}

	size_t HeapFactory::GetSize()
	{
		return heaps.size();
	}

	HeapFactory::HeapFactory(Token)
	{}
}
