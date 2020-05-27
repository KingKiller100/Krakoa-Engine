#pragma once

#include <vector>

namespace krakoa
{
	class Heap;

	using HeapList = std::vector<Heap*>;
	
	class HeapFactory final
	{
		struct Token {};
	public:
		HeapFactory(Token&&);

		static Heap* CreateHeap(const char* name);
		static Heap* GetDefaultHeap() noexcept;

		static const HeapList& GetHeaps();
		static void WalkTheHeap(const size_t index);
		
	private:
		static Heap* defaultHeap;
		static HeapList heaps;
	};
}
