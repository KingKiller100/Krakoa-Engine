#pragma once

#include "TemplateHeap.hpp"

#include <array>
#include <atomic>

namespace memory
{
	namespace
	{
		std::atomic<std::uint16_t> activeHeapIndex(1);
	}

	class HeapFactory final
	{
		using HeapList = std::array<Heap*, 128>;

		struct Token {};
	public:
		static void Initialize() noexcept;
		static void ShutDown() noexcept;

		template<typename T>
		static Heap* CreateHeap()
		{
			auto* heap = CAST(Heap*, malloc(sizeof(TemplateHeap<T>)));
			heap->Initialize(typeid(T).name(), &templateHeapVFTBL<T>);
			heaps[activeHeapIndex++] = heap;
			return heap;
		}

		static Heap* GetDefaultHeap() noexcept;
		static std::string WalkTheDefaultHeap();

		static const HeapList& GetHeaps();
		static std::string WalkTheHeap(const size_t index);

		static size_t GetSize();

		static void ReportMemoryLeaks();

	private:
		static void LogTotalBytes(const size_t* bytes) noexcept;
		static void LogTotalAllocations(const size_t* bytes) noexcept;
		static void ReportMemoryLeaks(Heap* const heap, const size_t minBookmark, const size_t maxBookmark);

		explicit HeapFactory(Token);

	private:
		static Heap* defaultHeap;
		static HeapList heaps;
	};
}
