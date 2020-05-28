#pragma once

#include <vector>

namespace memory
{
	class Heap;

	using HeapList = std::vector<Heap*>;
	
	class HeapFactory final
	{
		struct Token {};
	public:
		static void ShutDown() noexcept;

		static Heap* CreateHeap(const char* name);
		static Heap* GetDefaultHeap() noexcept;

		static const HeapList& GetHeaps();
		static void WalkTheHeap(const size_t index);

	private:
		explicit HeapFactory(Token);
		
	private:
		static Heap* defaultHeap;
		static HeapList heaps;
	};
}
