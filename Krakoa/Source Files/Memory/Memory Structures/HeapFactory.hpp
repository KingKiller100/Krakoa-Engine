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
			static Heap_VTBL tVTBL(GetTemplateHeapStatus<T>,
				CallObjectDestructor<T>);
			
			auto* heap = CAST(HeapBase*, malloc(sizeof(THeap<T>)));

			heap->Initialize(typeid(T).name(), &tVTBL);
			heaps.emplace_back(heap);
			return heap;
		}

		static HeapBase* GetDefaultHeap() noexcept;
		static std::string WalkTheDefaultHeap();

		static const HeapList& GetHeaps();
		static std::string WalkTheHeap(const size_t index);

		static size_t GetSize();

	private:
		explicit HeapFactory(Token);

	private:
		static HeapBase* defaultHeap;
		static HeapList heaps;
	};
}
