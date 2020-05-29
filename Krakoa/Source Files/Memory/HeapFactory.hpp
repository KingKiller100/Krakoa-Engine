#pragma once

#include <vector>

namespace memory
{
	class Heap;


	class HeapFactory final
	{
		using HeapList = std::vector<Heap*>;
		
		struct Token {};
	public:
		static void Initialize() noexcept;
		static void ShutDown() noexcept;

		static Heap* CreateHeap(const char* name);

		static Heap* GetDefaultHeap() noexcept;
		static std::string WalkTheDefaultHeap();

		static const HeapList& GetHeaps();
		static std::string WalkTheHeap(const size_t index);

		static size_t GetSize();

	private:
		explicit HeapFactory(Token);

	private:
		static Heap* defaultHeap;
		static HeapList heaps;
	};
}
