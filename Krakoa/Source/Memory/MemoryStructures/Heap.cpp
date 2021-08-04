#include "Precompile.hpp"
#include "Heap.hpp"

#include "AllocHeader.hpp"

#include "../MemoryDebug.hpp"

#include "../../Util/Fmt.hpp"
#include <Utility/Debug/kDebugger.hpp>

namespace memory
{
	size_t Heap::s_TotalLifetimeBytesAllocated = 0;

	using namespace patterns;

	void Heap::Initialize(const char* n, Heap_VFTBL* heapVTBL) noexcept
	{
		name = n;
		totalBytes = 0;
		allocListVoid = nullptr;
		vftbl = heapVTBL;
		allocListVoid = static_cast<BiDirectionalLinkedList<AllocHeader>*>(std::malloc(
			sizeof(BiDirectionalLinkedList<AllocHeader>)));
		std::memset(allocListVoid, 0, sizeof(BiDirectionalLinkedList<AllocHeader>));
		// family.pParent = nullptr;
		// family.pFirstChild = nullptr;
		// family.pPrevSibling = nullptr;
		// family.pNextSibling = nullptr;
	}

	void Heap::ShutDown() const
	{
		const auto leaks = WalkTheHeap();
		
		MEM_ASSERT(leaks == 0,
			util::Fmt("{0} has {1} remaining allocation(s)", name, leaks));
		DeleteLeaks();
		
		std::free(allocListVoid);
	}

	// bool Heap::AddToParent(Heap* pParent)
	// {
		// return false;
	// }

	void Heap::SetName(const char* n) noexcept
	{
		name = n;
	}

	const char* Heap::GetName() const noexcept
	{
		return name;
	}

	// const Heap::Family& Heap::GetFamily() const noexcept
	// {
		// return family;
	// }

	// Heap::Family& Heap::GetFamily() noexcept
	// {
		// return family;
	// }

	void Heap::Allocate(const size_t bytes) noexcept
	{
		totalBytes += bytes;
	}

	void Heap::Deallocate(const size_t bytes) noexcept
	{
		totalBytes -= bytes;
	}

	size_t Heap::GetTotalAllocatedBytes() const noexcept
	{
		return totalBytes;
	}

	size_t Heap::WalkTheHeap() const
	{
		auto [head, tail] = *static_cast<AllocHeaderLinkedList*>(allocListVoid);
		
		if (!head)
			return 0;

		size_t count(1);
		auto* current = head;

		while (current != tail)
		{
			current->data.VerifyHeader(true);
			if (current == nullptr)
				klib::kDebug::BreakPoint();
			
			current = current->next;
			++count;
		}

		return count;
	}

	void Heap::DeleteLeaks() const
	{
		auto* allocList = static_cast<BiDirectionalLinkedList<AllocHeader>*>(allocListVoid);
		if (!allocList->head)
			return;

		while (allocList->tail != allocList->head)
		{
			auto pPrev = allocList->tail->prev;
			// auto* ptr = static_cast<kmaths::Byte_Type*>(GetDataPointerFromNode(allocList->tail));
			// delete ptr;
			std::free(allocList->tail);
			allocList->tail = pPrev;
		}

		// auto lastPtr = GetDataPointerFromNode(allocList->head);
		// delete lastPtr;
		std::free(allocList->head);
		allocList->head = allocList->tail = nullptr;
	}

	size_t Heap::GetLastBookmark() const
	{
		auto* allocList = static_cast<BiDirectionalLinkedList<AllocHeader>*>(allocListVoid);
		if (allocList->tail)
			return allocList->tail->data.bookmark;

		return 0;
	}
	
	std::string Heap::GetStatus() const
	{
		MEM_ASSERT(vftbl->getStatusFunc != nullptr,
			"Heap's vftbl is empty");
		return vftbl->getStatusFunc(this);
	}
}
