#pragma once

#include <vector>

#include "TemplateHeap.hpp"

namespace memory
{
	class HeapFactory final
	{
		using HeapList = std::vector<HeapBase*>;

		struct Token {};
	public:
		static void Initialize() noexcept;
		static void ShutDown() noexcept;

		template<typename T>
		static HeapBase* CreateHeap()
		{
			auto* heap = CAST(HeapBase*, malloc(sizeof(TemplateHeap<T>)));
			heap->Initialize(typeid(T).name(), &templateHeapVFTBL<T>);
			heaps.emplace_back(heap);
			return heap;
		}

		static HeapBase* GetDefaultHeap() noexcept;
		static std::string WalkTheDefaultHeap();

		static const HeapList& GetHeaps();
		static std::string WalkTheHeap(const size_t index);

		static size_t GetSize();

		void ReportMemoryLeaks(HeapBase* const heap, const size_t minBookmark, const size_t maxBookmark);
		
	private:
		static void LogTotalBytes(const size_t* bytes) noexcept;
		static void LogTotalAllocations(const size_t* bytes) noexcept;
		
		explicit HeapFactory(Token);

	private:
		static HeapBase* defaultHeap;
		static HeapList heaps;
	};
}
