#pragma once

#include "TemplateHeap.hpp"
#include "../MemoryUtil.hpp"

#include <array>
#include <atomic>

namespace memory
{
	class HeapFactory final
	{
		static constexpr size_t ListSize = 128;
		
		using HeapList = std::array<Heap*, ListSize>;
		
		struct Token {};
	public:
		template<typename T>
		static Heap* CreateHeap()
		{
			auto* heap = CreateHeapImpl(typeid(T).name(), sizeof(BasicHeap<T>), &templateHeapVFTBL<T>);
			return heap;
		}

		// static bool AddToParent(const char* parentName, Heap* pChild);
		
		static void Initialize() noexcept;
		static void ShutDown() noexcept;

		static Heap* GetDefaultHeap() noexcept;
		static std::string WalkTheDefaultHeap();

		static const HeapList& GetHeaps();
		static std::string WalkTheHeap(const size_t index);

		static size_t GetSize();

		static void ReportMemoryLeaks();

	private:
		static Heap* CreateHeapImpl(const char* name, size_t bytes, Heap_VFTBL* vtbl);
		static void LogTotalBytes(const size_t bytes) noexcept;
		static void LogTotalAllocations(const size_t active, const size_t total) noexcept;
		static void ReportMemoryLeaks(Heap* const heap);

		static Heap* FindHeap(const char* name);
		
		explicit HeapFactory(Token);

	private:
		static Heap* defaultHeap;
		static HeapList heaps;
		static std::atomic_uint16_t activeHeapIndex;
	};
}
